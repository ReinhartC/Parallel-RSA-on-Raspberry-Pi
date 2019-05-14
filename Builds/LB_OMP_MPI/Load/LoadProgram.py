"""
Created on Tue Apr 16 15:16:47 2019

@author: Ivan A P
"""
from PIL import Image

def main ():

    filename = "img.jpg"

    im = Image.open(filename)
    new_size = (3456, 4700)
    new_im = Image.new("RGB", new_size, (0,0,0))
    empty = new_im.height - im.height
    fill = int((new_im.height/2) / (empty/2))
    diff = int((empty)/2)
    print ("size :" , im.size)
    print ("empty pixel per column :", empty)
    print ("empty pixel per part :", diff)
    print ("one new pixel every", fill, "pixel")

    a = new_im.load()
    b = im.load()


    for i in range (new_im.width//2):
        k = 0
        print0 = 0
        total_fill = int(((diff / (new_im.width / 2)) * i))
        for j in range (int((diff - (diff / (new_im.width / 2)) * i)), new_im.height//2):
            if print0 < fill:
                a[i,j] = b[i,k]
                k += 1
                print0 += 1
            else:
                if total_fill > 0:
                    a[i,j] = (0,0,0)
                    print0 = 0
                    total_fill -= 1
                else:
                    a[i,j] = b[i,k]
                    k += 1
                    print0 += 1

    for i in range (new_im.width//2):
        k = im.height-1
        print0 = 0 
        total_fill = int(((diff / (new_im.width / 2)) * i))
        diffa.append(total_fill)

        for j in range (new_im.height - 1 - int(diff - ((diff / (new_im.width // 2)) * i)), new_im.height//2 - 1 , -1):
            if print0 < fill:
                a[i,j] = b[i,k]
                k -= 1
                print0 += 1
            else:
                if total_fill > 0:
                    a[i,j] = (0,0,0)
                    print0 = 0
                    total_fill -= 1
                else:
                    a[i,j] = b[i,k]
                    k-= 1
                    print0 += 1


    for i in range (new_im.width//2 , new_im.width ):
        k = 0
        print0 = 0
        total_fill = int(diff - ((diff / (new_im.width / 2)) * (i - new_im.width //2)))

        for j in range (int(- diff + ((2 * diff / (new_im.width)) * i))  , new_im.height//2):

            if print0 < fill:
                a[i,j] = b[i,k]
                k += 1
                print0 += 1
            else:
                if total_fill > 0:
                    a[i,j] = (0,0,0)
                    print0 = 0
                    total_fill -= 1
                else:
                    a[i,j] = b[i,k]
                    k += 1
                    print0 += 1


    for i in range (new_im.width // 2  , new_im.width ):
        k = im.height - 1

        total_fill =  int(diff - ((diff / (new_im.width / 2)) * (i - new_im.width //2)))
        diffb.append(total_fill)
        for j in range (new_im.height -1 - int(- diff + ((2 * diff / (new_im.width)) * i)), new_im.height//2 - 1, -1):
            if print0 < fill:
                a[i,j] = b[i,k]
                k -= 1
                print0 += 1
            else:
                if total_fill > 0:
                    a[i,j] = (0,0,0)
                    print0 = 0
                    total_fill -= 1
                else:
                    a[i,j] = b[i,k]
                    k -= 1
                    print0 += 1

   for i in range (1, new_im.width//2):
       for j in range (1, new_im.height//2):
           if a[i,j] == (0,0,0):
               neighbors = [a[i-1,j-1], a[i,j-1], a[i+1,j-1], a[i-1,j],
                                a[i,j+1], a[i-1,j+1]]
               sum_of_neighbors = reduce(lambda x, y: np.add(np.array(x), np.array(y)),
                                         neighbors)
               avg = np.array(sum_of_neighbors) // 6
               a[i,j] = tuple(avg)

   for i in range (1, new_im.width//2):
       for j in range (new_im.height//2, new_im.height - 1):
           if a[i,j] == (0,0,0):
               neighbors = [a[i-1,j-1], a[i,j-1], a[i-1,j], a[i-1,j+1],
                                a[i,j+1], a[i+1,j+1]]
               sum_of_neighbors = reduce(lambda x, y: np.add(np.array(x), np.array(y)),
                                         neighbors, [0, 0, 0])
               avg = np.array(sum_of_neighbors) // 6
               a[i,j] = tuple(avg)

   for i in range (new_im.width//2, new_im.width -1):
       for j in range (1, new_im.height//2 ):
           if a[i,j] == (0,0,0):
               neighbors = [a[i-1,j-1], a[i,j-1], a[i-1,j], a[i-1,j+1],
                                a[i,j+1], a[i+1,j+1]]
               sum_of_neighbors = reduce(lambda x, y: np.add(np.array(x), np.array(y)),
                                         neighbors, [0, 0, 0])
               avg = np.array(sum_of_neighbors) // 6
               a[i,j] = tuple(avg)

   for i in range (new_im.width//2, new_im.width -1):
       for j in range (new_im.height//2, new_im.height -1):
           if a[i,j] == (0,0,0):
               neighbors = [a[i-1,j-1], a[i,j-1], a[i+1,j-1], a[i-1,j],
                                a[i-1,j+1], a[i,j+1]]
               sum_of_neighbors = reduce(lambda x, y: np.add(np.array(x), np.array(y)),
                                         neighbors, [0, 0, 0])
               avg = np.array(sum_of_neighbors) // 6
               a[i,j] = tuple(avg)

   crop_size = im.size
   crop_image = Image.new("RGB", crop_size, (0,0,0))
   crop_image.paste(new_im, ((crop_size[0] - new_im.size[0]) // 2,
                     (crop_size[1] - new_im.size[1]) // 2))

   crop_image.save('revisi.jpg')
   print("Done")

if ( __name__ == "__main__"):
    main()
