import cv2
import numpy as np

src_depth = cv2.imread('screenShot_depth1.jpg',cv2.IMREAD_GRAYSCALE)
src_rgb_from_file = cv2.imread('screenShot_rgb1.jpg',cv2.IMREAD_COLOR)
src_depth = (255-src_depth)

# cv2.imshow('origin_depth', src_depth)
# cv2.imshow('origin_rgb', src_rgb_from_file)

# y=200
# x=200
# t = np.array([src_depth[y][x]-src_depth[y][x-1],src_depth[y][x]-src_depth[y-1][x],src_depth[y][x]-src_depth[y+1][x],src_depth[y][x]-src_depth[y][x+1]])
# print t
# print type(t)
# blur_depth = cv2.GaussianBlur(src_depth,(5,5),0)
# grad_depth = np.zeros(src_depth.shape).astype(np.uint8)
# h,w = np.shape(src_depth)
# for y in range(h):
# 	for x in range(w):
# 		if y-1<0 or y+1>=h or x-1<0 or x+1>=w:
# 			grad_depth[y][x] = src_depth[y][x]
# 			continue
# 		neighbor = np.array([int(src_depth[y][x])-int(src_depth[y][x-1]),int(src_depth[y][x])-int(src_depth[y-1][x]),int(src_depth[y][x])-int(src_depth[y+1][x]),int(src_depth[y][x])-int(src_depth[y][x+1])])
# 		diff_t = np.amin(neighbor)
# 		if diff_t<-4:
# 			grad_depth[y][x] = 0 #np.amax([0,src_depth[y][x]+20*diff_t])
# 		else:
# 			grad_depth[y][x] = src_depth[y][x]

# cv2.imshow('gradient',grad_depth)


# blur2_depth = cv2.medianBlur(grad_depth,3)
# blur_depth = cv2.medianBlur(src_depth,9)

# kernel = np.array([[1,1,1,1], 
# 				   [1,1,1,1], 
# 				   [1,1,1,1], 
# 				   [1,1,1,1]],dtype=np.uint8)
# blur_depth = cv2.filter2D(src_depth, -1, kernel)
# _,bin_depth = cv2.threshold(blur_depth,1,255,cv2.THRESH_BINARY)
kernelOpen = np.ones((5,5))
kernelClose = np.ones((15,15))
maskOpen = cv2.morphologyEx(src_depth,cv2.MORPH_OPEN,kernelOpen)
maskClose = cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
# cv2.imshow('mask_open', maskOpen)
# cv2.imshow('mask_close', maskClose)
maskFinal = maskClose
_,conts,_ = cv2.findContours(maskFinal,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
# cv2.drawContours(src_rgb_from_file,conts,-1,(0,255,0),3)

rows,cols = src_depth.shape
for i in range(len(conts)):
	x,y,w,h = cv2.boundingRect(conts[i])
	x = max(0,x-10)
	y = max(0,y-10)
	w = min(cols-x,w+30)
	h = min(rows-y,h+30)
	cv2.rectangle(maskFinal,(x,y),(x+w,y+h),(255,0,0),-1)
# cv2.imshow('masked_area',maskFinal)


_,bin_depth = cv2.threshold(maskFinal,1,255,cv2.THRESH_BINARY)
src_rgb = cv2.bitwise_and(src_rgb_from_file,src_rgb_from_file,mask=bin_depth)
cv2.imshow('masked_rst',src_rgb)
# src_hsv = cv2.cvtColor(src_rgb,cv2.COLOR_BGR2HSV)
# cv2.imshow('masked_hsv', src_hsv)
src_hsv = cv2.cvtColor(src_rgb,cv2.COLOR_BGR2HSV)
hsv_boundaries = [([140, 50, 50],  [170, 255, 255])]		#orange 0 30
		  		  # ([30, 50, 50],    [60, 255, 255])]	#green  30 90
		  		  # ([94, 80, 2], 	[126, 255, 255]),	#blue   90 120
		  		  # ([103, 86, 65],   [145, 133, 128]),	#purple 120 140
		  		  # ([103, 86, 65],   [145, 133, 128]),	#pink
		  		  # ([103, 86, 65],   [145, 133, 128]),	#yellow
		  		  # ([103, 86, 65],   [145, 133, 128]),	#red 140 170
		  		  # ([103, 86, 65],   [145, 133, 128])]	#black
block_cts = np.array([])
block_cnt = 0
for (lower,upper) in hsv_boundaries:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "uint8")
	upper = np.array(upper, dtype = "uint8")
	kernelOpen = np.ones((4,4))
	kernelClose = np.ones((8,8))
	mask = cv2.inRange(src_hsv,lower,upper)
	cv2.imshow('mask',mask)
	maskOpen = cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
	maskClose = cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
	# cv2.imshow('mask',mask_close)
	maskFinal = maskClose

	minArea = 350
	_,conts,_ = cv2.findContours(maskFinal,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
	for i in range(len(conts)):
		x,y,w,h = cv2.boundingRect(conts[i])
		if cv2.contourArea(conts[i])<minArea:
			block_cnt = block_cnt - 1
			continue
		cv2.drawContours(src_rgb,conts[i],-1,(0,255,0),3)
		block_cts = np.append(block_cts,[x+float(w)/2,y+float(h)/2])
	block_cnt = block_cnt + len(conts)

block_centers = block_cts.reshape(block_cnt,2)
print(block_centers)
cv2.imshow('rst',src_rgb)
# hsv_boundaries = [([161, 155, 84],  [179, 255, 255]),		#red
# 				  ([25, 52, 72],    [102, 255, 255]),		#green
# 				  ([94, 80, 2], 	[126, 255, 255]),		#blue
# 				  ([103, 86, 65],  [145, 133, 128]),	#purple
# 				  ([103, 86, 65],  [145, 133, 128]),	#pink
# 				  ([103, 86, 65],  [145, 133, 128]),	#yellow
# 				  ([103, 86, 65],  [145, 133, 128]),	#orange
# 				  ([103, 86, 65],  [145, 133, 128])]	#black

# # for (lower,upper) in rgb_boundaries:
# # 	# create NumPy arrays from the boundaries
# # 	lower = np.array(lower, dtype = "uint8")
# # 	upper = np.array(upper, dtype = "uint8")
 
# # 	# find the colors within the specified boundaries and apply
# # 	# the mask
# # 	mask = cv2.inRange(image, lower, upper)
# # 	output = cv2.bitwise_and(image, image, mask = mask)
 
# # create NumPy arrays from the boundaries
# lower = np.array([90, 60, 2], dtype = "uint8")
# upper = np.array([130, 255, 255], dtype = "uint8")

# # find the colors within the specified boundaries and apply
# # the mask
# mask = cv2.inRange(src_hsv, lower, upper)
# output = cv2.bitwise_and(src_hsv, src_hsv, mask = mask)
# output_rgb = cv2.cvtColor(output,cv2.COLOR_HSV2BGR)
# blur_gray = cv2.cvtColor(output_rgb,cv2.COLOR_BGR2GRAY)

# # cv2.imshow('bin',bin_depth)
# # cv2.imshow('rst',rst)
# # cv2.imshow('blur2',blur2_depth)


# blur_gray = cv2.medianBlur(blur_gray,3)
# # _,blur_bin = cv2.threshold(blur_gray,1,255,cv2.THRESH_BINARY)
# cv2.imshow('output',output_rgb)
# cv2.imshow('blur_gray',blur_gray)
# cv2.imshow('blur_bin',blur_bin)
# kernel = np.array([[-1,-1,-1,-1], [-1,5,5,-1], [-1,5,5,-1], [-1,-1,-1,-1]])
# sharpened_depth = cv2.filter2D(blur_depth, -1, kernel)
# cv2.imshow('sharpened',sharpened_depth)


# Setup SimpleBlobDetector parameters.
# params = cv2.SimpleBlobDetector_Params()
# # # Change thresholds
# params.minThreshold = 0
# params.maxThreshold = 255
# # Filter by Area.
# params.filterByArea = True
# params.minArea = 200
# params.maxArea = 900
# # Filter by Circularity
# params.filterByCircularity = False
# params.minCircularity = 0.55
# params.maxCircularity = 0.9
# # # Filter by Convexity
# params.filterByConvexity = False
# # params.minConvexity = 0.87
# # Filter by Inertia
# params.filterByInertia = False
# params.minInertiaRatio = 0.4

# # Create a detector with the parameters
# ver = (cv2.__version__).split('.')
# if int(ver[0]) < 3 :
# 	detector = cv2.SimpleBlobDetector(params)
# else : 
# 	detector = cv2.SimpleBlobDetector_create(params)

# # Detect blobs.
# keypoints = detector.detect(blur_gray)
# cnt = 0
# centers = []
# for point in keypoints:
# 	centers = np.append(centers,[point.pt[0],point.pt[1]])
# 	cnt = cnt + 1
# print cnt
# # print centers
# # centers = centers.reshape(cnt,2).astype(int)
# # print type(centers[0][0])
# # print centers.size
# # # contours = np.array([])
# # # print type(contours)
# # # for point in keypoints:
# # # 	contours = np.append(contours,np.array([point.pt[0],point.pt[1]]))
	
# # # print contours
# # # print contours.shape

# # Draw detected blobs as red circles.
# # cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures
# # the size of the circle corresponds to the size of blob

# rst = cv2.drawKeypoints(blur_gray, keypoints, np.array([]), (0,255,0), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# ret, thresh = cv2.threshold(blur_depth, 20, 255, 0)
# thresh = (255-thresh)
# # cv2.imshow('thresh',thresh)
# _,contours,_ = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
# print type(contours)
# print contours
# for contour in contours:
# 	cv2.drawContours(src_depth,contour,-1,(0,255,0),3)

# cv2.imshow('depth',src_depth)
# # cv2.imshow('rgb',src_rgb)
# cv2.imshow('rst',rst)
cv2.waitKey(0)
cv2.destroyAllWindows()



