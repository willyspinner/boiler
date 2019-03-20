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

    -i : specify information for usage (you will edit this using EDITOR)
    -s : specify scripts to be run for usage (you will edit this using EDITOR)
    
\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
uninstall BOILERPLATE_NAME
```
remove a boilerplate from boiler. note that this is permanent.

- where BOILERPLATE_NAME is the name of your boilerplate in boiler

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
add BOILERPLATE_NAME
```
copies the BOILERPLATE_NAME boilerplate to the current working directory.

- where BOILERPLATE_NAME is the name of your boilerplate in boiler

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
edit BOILERPLATE_NAME
```
edit the file of BOILERPLATE_NAME. Note that this can't be a directory.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

# Examples:

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler install ~/willysBinSrc/reactcomponents/reactbutton.js
```
install a react button as a new boilerplate called react/button.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler ls
```
list out boilerplates. In this case, you would see react/button.

\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

```sh
$ boiler add react/button
```
add a react/button boiler plate to current working directory.
> Made by Wilson Jusuf.

# requirements:
* tree 

To get this, simply: 

```sh
brew install tree || sudo apt-get install tree # for mac or ubuntu respectively.
```

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
- [ ] Don't use the bash `tree`! 
- [ ] Makefile for installation

# Done:
- [x] Support for running init scripts every time boilerplate is added (e.g. npm install dependencies ... etc)
- [x] Support for catting txt info files that outline the usage of the boilerplate every time the `add` command is run.

