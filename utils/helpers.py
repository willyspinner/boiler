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
        cprint('boiler: '+t,'red',attrs=['bold'])

def separator():
    return '------------'

