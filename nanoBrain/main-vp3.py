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

    with input.rlock_cpu() as in_data, min_coords.rlock_cpu() as min_data, max_coords.rlock_cpu() as max_data:
        print(min_data.shape)
        print(max_data.shape)
        print(max_data)
        min_loc = tuple(min_data[0].astype(int)[::-1])
        max_loc = tuple(max_data[0].astype(int)[::-1])
        min_locs = min_data

    min_value = in_data[min_loc]
    max_value = in_data[max_loc]



    print("--- %s seconds ---" % (time.time() - start_time))
x = max_loc[1]
y = max_loc[0]
r = 20
with output.rlock_cpu() as outData:
    
    # Image.fromarray(outData).save('tutorial_blurred_python.png')
    outIm = Image.fromarray(outData)
    draw = ImageDraw.Draw(outIm)

    draw.ellipse((x-r, y-r, x+r, y+r), fill=0)
    outIm.save('test_out.png')
    
    
    
