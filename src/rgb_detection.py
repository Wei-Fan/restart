import cv2
import numpy as np
src_rgb = cv2.imread('screenShot_rgb6.jpg',cv2.IMREAD_COLOR)
src_rgb_show = cv2.cvtColor(src_rgb,cv2.COLOR_BGR2RGB)
cv2.imshow('src',src_rgb_show)
# rows,cols = src_rgb.shape[:2]
src_r,src_g,src_b = cv2.split(src_rgb)

src_r = src_r.astype(np.int16)
src_b = src_b.astype(np.int16)
src_g = src_g.astype(np.int16)
r_b = src_r - src_b
b_g = src_b - src_g
g_r = src_g - src_r

# print rows,cols
		  #[r-b, b-g, g-r]
rgb_gap = [([20, 2, -255],  [255, 255, -20]),	#red & pink
		  ([20, -255, -255], [255, -5, -15]),	#orange
  		  # ([80, 10, -255], [255, 255, -10]),	#pink
  		  ([-255, 20, 20], 	[-20, 255, 255]),	#blue
  		  ([-255, 10, -255], [-1, 255, -10]),	#purple
  		  ([-40, -255, 15],  [20, -15, 255])]	#green
  		  # ([-40, -40, -40],   [40, 40, 40])]	#black
  		  # ([103, 86, 65],   [145, 133, 128]),	#yellow
# rgb_boundaries = [([140, 20, 50],  	[220, 115, 140]),	#red
# 				  ([180, 80, 30], 	[255, 170, 170]),	#orange
# 		  		  ([220, 80, 140], [255, 140, 180]),	#pink
# 		  		  ([60, 90, 160], 	[90, 120, 220]),	#blue
# 		  		  ([110, 80, 130], [160, 130, 180]),	#purple
# 		  		  ([80, 150, 100],  [130, 190, 150]),	#green
# 		  		  ([50, 50, 50],   [100, 120, 130])]	#black
		  		  # ([103, 86, 65],   [145, 133, 128]),	#yellow
block_cts = np.array([])
block_cnt = 0
cnt = 0
for (lower,upper) in rgb_gap:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "int16")
	upper = np.array(upper, dtype = "int16")

	r_b_bool = np.logical_and(r_b<=upper[0],r_b>=lower[0])
	b_g_bool = np.logical_and(b_g<=upper[1],b_g>=lower[1])
	g_r_bool = np.logical_and(g_r<=upper[2],g_r>=lower[2])
	mask_bool1 = np.logical_and(r_b_bool,b_g_bool)
	mask_bool2 = np.logical_and(mask_bool1,g_r_bool)

	mask = mask_bool2*255
	mask = mask.astype(np.uint8)
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

	minArea = 300
	_,conts,_ = cv2.findContours(maskFinal,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_NONE)
	for i in range(len(conts)):
		x,y,w,h = cv2.boundingRect(conts[i])
		if cv2.contourArea(conts[i])<minArea:
			block_cnt = block_cnt - 1
			continue
		cv2.drawContours(src_rgb,conts[i],-1,(0,255,0),3)
		block_cts = np.append(block_cts,[x+float(w)/2,y+float(h)/2])
	block_cnt = block_cnt + len(conts)


"""detect black and yellow"""
yb_gap = [([40, 40, 40],  [100, 100, 100]),	#black
		  ([245, 245, 100], [255, 255, 255])]	#yellow
for (lower,upper) in yb_gap:
	# create NumPy arrays from the boundaries
	lower = np.array(lower, dtype = "int16")
	upper = np.array(upper, dtype = "int16")

	r_bool = np.logical_and(src_r<=upper[0],src_r>=lower[0])
	g_bool = np.logical_and(src_g<=upper[1],src_g>=lower[1])
	b_bool = np.logical_and(src_b<=upper[2],src_b>=lower[2])
	mask_bool1 = np.logical_and(r_bool,b_bool)
	mask_bool2 = np.logical_and(mask_bool1,g_bool)

	mask = mask_bool2*255
	mask = mask.astype(np.uint8)
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

	minArea = 380
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



