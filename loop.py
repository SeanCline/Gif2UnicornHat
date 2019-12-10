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

def base_filename(path):
    return os.path.splitext(os.path.basename(path))[0]

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
        
        basename = base_filename(gif) # Name without file extension.
        if basename.endswith('_skip'):
            print('Skipping:', gif)
            continue

        playtime = 120 if basename.endswith('_anim') else 15
        
        print('Displaying:', gif, 'Time:', playtime)
        proc = subprocess.Popen(['./Gif2UnicornHat', gif, '.02'])
        time.sleep(playtime)
        proc.terminate()
