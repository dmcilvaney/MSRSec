# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT License.

import subprocess, os, sys, json

# This python script will dynamically generate a cgmanifest file besed on a template (or generate from
# scratch if needed). It will also automatically populate the the cgmanifest file with the first level of submodules
# included in the current repo.
#
# The template file can include:
#   - explicit registrations (as defined in the Component Governance docs)
#   - git registrations which omit the 'commitHash' tag and replace it with a 'branch' tag
#   - no registrations at all (you can simpmly exclude the cgmanifest file completely in this case)
#
# The script will:
#   - Leave explicit registrations as is
#   - Convert all 'branch' tags to 'commitHash' tags based on a call to the upstream repo via 'git ls-remote'
#   - Add an entry for each git submodule not already added by either of the above steps

# Returns a list of tupples (URL, Commit hash) of registered first level submodules
def get_submodules():
    submodules = []
    git_process = subprocess.run(["git", "submodule"],shell=False, stdout=subprocess.PIPE,  universal_newlines=True, timeout=10)
    git_process.check_returncode()

    # Parse list of submodules to get commit hash and relative path
    for line in git_process.stdout.splitlines():
        print (line)
        hash = line.split()[0]
        path = os.path.abspath(os.path.join(os.getcwd(), line.split()[1]))
        
        # Change cwd to each submodule, query for upstream url
        submodule_process = subprocess.run("git config --get remote.origin.url", cwd=path, shell=True, stdout=subprocess.PIPE,  universal_newlines=True, timeout=10)
        submodule_process.check_returncode()

        url = submodule_process.stdout.splitlines()[0]
        repo = (url, hash)
        submodules.append(repo)

    return submodules

# Replaces branch tags with the latest commit hash for that branch
def update_git_repo(repo):
    branch = repo.pop('branch')
    arg = "ls-remote " + repo['repositoryUrl'] + " " + branch

    git_process = subprocess.run("git " + arg, shell=True, stdout=subprocess.PIPE,  universal_newlines=True, timeout=10)
    git_process.check_returncode()

    hash = git_process.stdout.splitlines()[0].split()[0]
    repo['commitHash'] = hash

    print("\t"+repo['repositoryUrl'], branch)
    print("\tUpstream is on commit", hash)

# Searches for and updates all git repos which have branches instead of commit hashes
def update_registration(registration):
    component = registration['component']
    if component['type'] == 'git':
        git = component['git']
        if 'commitHash' not in git:
            print("Found registration with no commitHash tag")
            update_git_repo(git)

# Adds a new git component to the registrations
def add_new_registration(registrations, url, hash):
    new_registration = dict()    
    new_component = dict()
    new_git = dict()

    new_git['repositoryUrl'] = url
    new_git['commitHash'] = hash

    new_component['type'] = 'git'
    new_component['git'] = new_git

    new_registration['component'] = new_component

    registrations.append(new_registration)

# Return true if a (url,hash) pair already exists in the json
def check_existing_registration(registrations, url, hash):
    # Strip .git from URLs
    if url.endswith('.git'):
        url = url[:-4]

    print("Checking submodule", url, "with hash:", hash)
    for registration in registrations:
            component = registration['component']
            if component['type'] == 'git':
                git = component['git']

                # Strip .git from URLs
                component_url = git['repositoryUrl']
                if component_url.endswith('.git'):
                    component_url = component_url[:-4]

                if (('commitHash' in git) and 
                        (git['commitHash'] == hash) and
                        (component_url == url)):
                    return True
    return False

def default_cgmanifest():
    new_manifest = dict()
    new_manifest['Registrations'] = list()
    new_manifest['Version'] = 1

    return new_manifest

def update_json(json_file_in, json_file_out):
    #Load json, then search for any repos with branches.
    if json_file_in is not None:
        print("Opening file ", os.path.abspath(os.path.join(os.getcwd(), json_file_in)))
        with open(json_file_in, 'r') as f_in:
            cgmanifest = json.load(f_in)
    else:
        print("Starting with blank cgmanifest")
        cgmanifest = default_cgmanifest()

    registrations = cgmanifest["Registrations"]

    # Update existing registrations to replace 'branch' with 'commitHash'
    for registration in registrations:
        update_registration(registration)

    # Add any missing submodules
    for submodule in get_submodules():
        if not check_existing_registration(registrations, submodule[0], submodule[1]):
            print("\tRegistering new submodule")
            add_new_registration(registrations, submodule[0], submodule[1])
        else:
            print("\tSubmodule already registered")

    print("Writing back to file ", os.path.abspath(os.path.join(os.getcwd(), json_file_out)))
    with open(json_file_out, 'w') as f_out:
        json.dump(cgmanifest, f_out, indent=4)


json_file_in = "cgmanifest_template.json"
json_file_out = "cgmanifest.json"

num_args = len(sys.argv)
if num_args == 1:
    print("Using default filenames")
if num_args >= 2:
    json_file_in = sys.argv[1]
if num_args == 3:
    json_file_out = sys.argv[2]
if num_args > 3:
    print("\n\n\t\tUse: build_cgmanifest.py [input.json [output.json]]\n\n")
    raise ValueError("Too many arguments")

if not os.path.isfile(json_file_in):
    json_file_in = None

update_json(json_file_in, json_file_out)