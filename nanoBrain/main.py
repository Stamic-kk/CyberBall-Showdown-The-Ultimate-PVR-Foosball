import sys
import vpi
  
import numpy as np
from PIL import Image
from PIL import ImageDraw
from argparse import ArgumentParser
import time
import os


start_time = time.time()
 # 2. Parse command line parameters
 # -----------------------------------------------------------------------------
# parser = ArgumentParser()
# parser.add_argument('input',
#                      help='Image to be used as input')
# parser.add_argument('filter', help='if use filter')
# args = parser.parse_args()

make_used = 0
use_filter = False
arr = os.listdir('inputs/')
print(arr)

for im in arr:
    try:
        #input = vpi.asimage(np.asarray(Image.open(args.input).crop((0, 0, 3000, 2500))))
        with vpi.Backend.CUDA:
            input = vpi.asimage(np.asarray(Image.open('inputs/'+im)))
    except IOError:
        sys.exit("Input file not found")
    except:
        sys.exit("Error with input file")
    with vpi.Backend.CUDA:

        output = input.convert(vpi.Format.U8) \
                    # .box_filter(11, border=vpi.Border.ZERO)
        if use_filter:
            output = output.box_filter(5, border=vpi.Border.ZERO)
        # print(output.size)
        # print(output.box_filter(11, border=vpi.Border.ZERO).size)

    with vpi.Backend.CUDA:
        min_coords, max_coords = output.minmaxloc(min_capacity=10, max_capacity=10)
    for k in range(5):
        make_used += k
    with input.lock(), min_coords.lock(), max_coords.lock():

        min_loc = tuple(min_coords.cpu()[0].astype(int)[::-1])
        max_loc = tuple(max_coords.cpu()[0].astype(int)[::-1])
        min_locs = min_coords.cpu()

    min_value = input.cpu()[min_loc]
    max_value = input.cpu()[max_loc]


    print("--- %s seconds ---" % (time.time() - start_time))


# x = min_loc[1]
# y = min_loc[0]
# r = 100
# with output.lock():
    
#     # Image.fromarray(outData).save('tutorial_blurred_python.png')
#     outIm = Image.fromarray(output.cpu())
#     draw = ImageDraw.Draw(outIm)
#     print("x: ", x, "y: ", y)
#     draw.ellipse((x-r, y-r, x+r, y+r), fill=0)
#     outIm.save('outputs/out.png')
#     print(make_used)
    
    
    
