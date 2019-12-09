#!/usr/bin/env python3

import os
import time
import subprocess
import atexit
import random

def get_files_with_extension_recursive(extension, dir = '.'):
    for root, dirs, files in os.walk(dir):
        for file in files:
            if file.endswith(extension):
                yield os.path.join(root, file)

def get_gifs():
    return get_files_with_extension_recursive('.gif')

# Keep track of the currently open process so we can exit gracefully.
proc = None
def cleanup():
    if proc != None:
        proc.terminate()

atexit.register(cleanup)

# Loop over and display the gifs.
while True:
    # On each cycle, look for new gifs and shuffle them.
    gifs = list(get_gifs())
    random.shuffle(gifs)
    #print("Found gifs:", gifs)
    
    for gif in gifs:
        print('Displaying:', gif)
        if gif.endswith('_skip'):
            continue
        
        playtime = 120 if gif.endswith('_anim') else 15
        
        proc = subprocess.Popen(['./Gif2UnicornHat', gif, '.04', '2'])
        time.sleep(playtime)
        proc.terminate()
