import cv2
import numpy as np
src_rgb = cv2.imread('screenShot_rgb6.jpg',cv2.IMREAD_COLOR)
src_rgb_show = cv2.cvtColor(src_rgb,cv2.COLOR_BGR2RGB)
cv2.imshow('src',src_rgb_show)
rows,cols = src_rgb.shape[:2]
src_r,src_g,src_b = cv2.split(src_rgb)
print rows,cols
		  #[r-b, b-g, g-r]
rgb_gap = [([20, 1, -255],  [255, 255, -20]),	#red
		  ([15, -255, -255], [255, -5, -15]),	#orange
  		  # ([80, 10, -255], [255, 255, -10]),	#pink
  		  ([-255, 20, 20], 	[-20, 255, 255]),	#blue
  		  ([-255, 10, -255], [-1, 255, -10]),	#purple
  		  ([-40, -255, 15],  [20, -15, 255])]	#green
  		  # ([-40, -40, -40],   [40, 40, 40])]	#black
  		  # ([103, 86, 65],   [145, 133, 128]),	#yellow
rgb_boundaries = [([140, 20, 50],  	[220, 115, 140]),	#red
				  ([180, 80, 30], 	[255, 170, 170]),	#orange
		  		  ([220, 80, 140], [255, 140, 180]),	#pink
		  		  ([60, 90, 160], 	[90, 120, 220]),	#blue
		  		  ([110, 80, 130], [160, 130, 180]),	#purple
		  		  ([80, 150, 100],  [130, 190, 150]),	#green
		  		  ([50, 50, 50],   [100, 120, 130])]	#black
		  		  # ([103, 86, 65],   [145, 133, 128]),	#yellow
block_cts = np.array([])
block_cnt = 0
cnt = 0
for (lower,upper) in rgb_gap:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "int16")
	upper = np.array(upper, dtype = "int16")

	mask = np.zeros((rows,cols)).astype(np.uint8)
	for x in range(rows):
		for y in range(cols):
			r = np.int16(src_r[x][y])
			b = np.int16(src_b[x][y])
			g = np.int16(src_g[x][y])
			if lower[0]<r-b and r-b<upper[0] and lower[1]<b-g and b-g<upper[1] and lower[2]<g-r and g-r<upper[2]:
				mask[x][y] = 255
	# mask = cv2.inRange(src_rgb,lower,upper)
	showname = "mask{}"
	showname = showname.format(cnt)
	cnt = cnt + 1
	# cv2.imshow(showname,mask)


	kernelOpen = np.ones((3,3))
	kernelClose = np.ones((8,8))
	maskOpen = cv2.morphologyEx(mask,cv2.MORPH_OPEN,kernelOpen)
	maskClose = cv2.morphologyEx(maskOpen,cv2.MORPH_CLOSE,kernelClose)
	cv2.imshow(showname,maskClose)
	maskFinal = maskClose

	minArea = 200
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

cv2.waitKey(0)
cv2.destroyAllWindows()



