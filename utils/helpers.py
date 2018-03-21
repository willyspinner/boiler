import sys
from termcolor import colored, cprint
# styling stuff:
def text(t,style='normal'):
    if (style == 'normal'):
        cprint(t)
    elif (style == 'header'):
        cprint(t,'magenta')
        cprint(separator(),'magenta')
    elif (style == 'subheader'):
        cprint(t,'white',attrs=['bold'])
    elif (style == 'usage'):
        cprint(t,'cyan',attrs=['bold','blink'])
    elif (style =='ex'):
        cprint(t,'blue')
    elif(style =='error'):
        cprint('boiler: '+t,'white','on_red',attrs=['bold'])
    elif(style =='notif'):
        cprint('boiler: '+t,'white','on_blue',attrs=['bold'])

def separator():
    return '------------'

def getFlagArg(flag):
     """
     see if a flag exists, and if so get its value, otherwise return NULL
     """
     for i in range(2,len(sys.argv)):
         if sys.argv[i] == flag:
             if i == len(sys.argv) -1:
                 return 'FLAG_FOUND_LAST'
             return sys.argv[i + 1]
     return 'NULL'
