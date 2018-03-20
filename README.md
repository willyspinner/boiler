usage: boiler COMMAND [COMMAND_OPTIONS]
## List of COMMANDs:
\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-
```sh
ls
```
List your boilerplates.
\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-
```sh
install PATH/TO/YOUR/BOILERPLATE [-n BOILERPLATE_NAME]
```
Install a boilerplate to boiler to be reused .
    - where PATH/TO/YOUR/BOILERPLAtE is the directory, or code, of your boilerplate
    - where BOILERPLATE_NAME is the optional name of your boilerplate in boiler, can be "directorial"
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

## Examples:
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
> Made by Wilson Jusuf, Fearless Magicians.

