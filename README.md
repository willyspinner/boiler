A quick command line tool for storing boilerplate code files.

usage: 
```sh
boiler COMMAND [COMMAND_OPTIONS]
```
# List of COMMANDs:

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
ls
```
List your boilerplates.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
install PATH/TO/YOUR/BOILERPLATE [-n BOILERPLATE_NAME]
```

Install a boilerplate to boiler to be reused.

- where PATH/TO/YOUR/BOILERPLATE is the directory, or code, of your boilerplate.
- where BOILERPLATE_NAME is the optional name of your boilerplate in boiler, can be a whole directory.
    
Flags:

    -i : specify information new boilerplate (you will edit this using EDITOR)
    
\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
uninstall BOILERPLATE_NAME
```
remove a boilerplate from boiler. note that this is permanent.

- where BOILERPLATE_NAME is the name of your boilerplate in boiler

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
show BOILERPLATE_NAME
```
cat the boilerplate to stdout.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
edit BOILERPLATE_NAME
```
edit the file of BOILERPLATE_NAME using editor `VISUAL`.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

# Examples:

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler install ~/willysBinSrc/reactcomponents/reactbutton.js
```
install a react button as a new boilerplate called reactbutton.js

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler ls
```
list out boilerplates. In this case, you would see react/button.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler show reactbutton.js
```
see reactbutton.js boilerplate.

> Made by Wilson Jusuf.
# boilerconfig.json:
This will host all the configuration and location of all the boilerplates.
```json
{
    "boilerplates": [
        {
            "name": "name of boilerplate item.",
            "description": "description of the boilerplate",
            "boilerpath": "location relative to $BOILERDIR/boilerplates"
        }, ...
    ],
}
```
# requirements:
g++ / clang++ with c++11 support.

# TODO:
- [ ] revamp whole interface - keep data in json file (json file), directory.
    -  Data like:
        - installed boilerplates location and names
        - boiler plate names, etc.
    - use shell env `BOILERDIR` as location for all the boilerplate stuff.
        - if `BOILERDIR` not set, then prompt our user to set it first.
    - put default stuff.
- [ ] add configs for:
    - [ ] nginx-http (single file)
    - [ ] nginx-https (single file)
    - [ ] systemd .service conf (single file)
    - [ ] golang web server (single file)
    - [ ] nodejs web server (single file)
 - [ ] DISABLE init scripts - this really is not needed - let the client do it by him/herself.
- default command should print out the boilerplate to stdout. Add -o FILENAME file to save to FILENAME.
- [x] Don't use the bash `tree`!
- [x] Makefile for installation.
- [x] Possibly move to C++ to bundle everything into one, instead of python.

# Done:
- [x] Support for running init scripts every time boilerplate is added (e.g. npm install dependencies ... etc)
- [x] Support for catting txt info files that outline the usage of the boilerplate every time the `add` command is run.

