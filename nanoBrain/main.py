import sys
import vpi
  
import numpy as np
from PIL import Image
from PIL import ImageDraw
from argparse import ArgumentParser
import time
start_time = time.time()
 # 2. Parse command line parameters
 # -----------------------------------------------------------------------------
parser = ArgumentParser()
parser.add_argument('input',
                     help='Image to be used as input')
args = parser.parse_args()

for i in range (0, 1):
    try:
        #input = vpi.asimage(np.asarray(Image.open(args.input).crop((0, 0, 200, 200))))
        input = vpi.asimage(np.asarray(Image.open(args.input)))
    except IOError:
        sys.exit("Input file not found")
    except:
        sys.exit("Error with input file")
    with vpi.Backend.CUDA:

        output = input.convert(vpi.Format.U8) \
                    # .box_filter(11, border=vpi.Border.ZERO)
        print(output.size)
        print(output.box_filter(11, border=vpi.Border.ZERO).size)

    with vpi.Backend.CUDA:
        min_coords, max_coords = output.minmaxloc(min_capacity=100, max_capacity=100)
    
    with input.lock(), min_coords.lock(), max_coords.lock():

        min_loc = tuple(min_coords.cpu()[0].astype(int)[::-1])
        max_loc = tuple(max_coords.cpu()[0].astype(int)[::-1])
        min_locs = min_coords.cpu()

    min_value = input.cpu()[min_loc]
    max_value = input.cpu()[max_loc]


    print("--- %s seconds ---" % (time.time() - start_time))
x = max_loc[1]
y = max_loc[0]
r = 20
with output.lock():
    
    # Image.fromarray(outData).save('tutorial_blurred_python.png')
    outIm = Image.fromarray(output.cpu())
    draw = ImageDraw.Draw(outIm)

    draw.ellipse((x-r, y-r, x+r, y+r), fill=0)
    outIm.save('test_out.png')
    
    
    