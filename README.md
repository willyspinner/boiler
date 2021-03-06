A quick command line tool for storing boilerplate code files, configurations (e.g. systemd, nginx), etc.
We all forget them too easily sometimes.

usage: 
```sh
boiler COMMAND [COMMAND_OPTIONS]
```
# List of COMMANDS:

### List your boilerplates.
```sh
list (or ls)
```


### install a new boilerplate.
```sh
install PATH/TO/BOILERPLATE [-n BOILERPLATE_NAME] [ -d BOILERPLATE_DESCRIPTION] [-s,--soft or -h, --hard]
```
`boiler` will copy the boilerplate in `PATH/TO/BOILERPLATE` to `BOILERDIR/boilerplates`.  There are 3 ways to do this:
1. (default) copy
2. soft link (-s or --soft)
3. hard link (-h or --hard)

WARNING: as of now, we **cannot hardlink** to `boilerconfig.json`. Changes to boilerconfig.json will be saved atomically. To achieve this, we use the `rename()` system call to rename a temporary file we're writing to to `boilerconfig.json`. This means that its inode will always change.

### uninstall a boilerplate

```sh
uninstall BOILERPLATE_NAME [--cached or -c]
# NOTE: uninstall deletes the boilerplate file. Use the -c or --cached  flag to preservethe boilerplate file int the BOILERDIR/boilerplates directory. 
```

### print a boilerplate to stdout

```sh
show BOILERPLATE_NAME
```

### edit a boilerplate

```sh
edit BOILERPLATE_NAME
# edit the file of BOILERPLATE_NAME using editor `VISUAL`.
```


# boilerconfig.json:
This will host all the configuration and location of all the boilerplates.
```json
{
    "boilerplates": [
        {
            "name": "name of boilerplate item.",
            "description": "description of the boilerplate",
            "boilerpath": "location relative to $BOILERDIR/boilerplates"
        }, 
    ],
}
```
# how to use:

After putting the binary somewhere in your `PATH`, set the environment variable `BOILERDIR`, which is a directory `boiler` will use to put the boilerplates and configurations.

# requirements to build from source:
- g++ / clang++ with c++11 support
- nlohmann/json C++ json library


# TODO:
- [x] revamp whole interface - keep data in json file (json file), directory.
    -  Data like:
        - installed boilerplates location and names
        - boiler plate names, etc.
    - use shell env `BOILERDIR` as location for all the boilerplate stuff.
        - if `BOILERDIR` not set, then prompt our user to set it first.
    - put default stuff.
- [ ] add configs for:
    - [x] nginx-http (single file)
    - [ ] nginx-https (single file)
    - [ ] systemd .service conf (single file)
    - [ ] golang web server (single file)
    - [ ] nodejs web server (single file)
 - [x] DISABLE init scripts - this really is not needed - let the client do it by him/herself.
- [x] default command should print out the boilerplate to stdout. Add -o FILENAME file to save to FILENAME.
- [x] Don't use the bash `tree`!
- [x] Makefile for installation.
- [x] Possibly move to C++ to bundle everything into one, instead of python.

# Done:
- [x] Support for running init scripts every time boilerplate is added (e.g. npm install dependencies ... etc)
- [x] Support for catting txt info files that outline the usage of the boilerplate every time the `add` command is run.


> Made by Wilson Jusuf.