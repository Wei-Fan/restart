import cv2
import numpy as np
#from PyQt4.QtGui import QImage
#import freenect


def writeConfg():
	camera_matrix = [[1,2,3],[4,5,6],[7,8,9]]
	dist_coefs = np.array([[1,2,3,7],[4,5,6,8]])
	f = open('calibration.cfg', 'w')
	f.write("intrinsic matrix:\r\n")
	f.write(str(camera_matrix))
	f.write("\r\ndistortion coefficients:\r\n")
	f.write(str( dist_coefs.ravel()))
	f.close()
	
def loadCameraCalibration():
	"""
    TODO:
    Load camera intrinsic matrix from file.
    """
	try:
		cam_file = open('calibration.cfg','r')
	except Exception as e:
		print "Cannot Open .cfg file"
		exit()
	file_lines = cam_file.readlines()

	intrinsic_matrix_str = file_lines[1:4]
	intrinsic_matrix_str_lines = [line.strip() for line in intrinsic_matrix_str]
	intrinsic_matrix = []
	for x in intrinsic_matrix_str_lines:
		x = x.replace('[','')
		x = x.replace(']','')
		x_t = np.fromstring(x,sep=' ')
		intrinsic_matrix = np.append(intrinsic_matrix, x_t)
	intrinsic_matrix = intrinsic_matrix.reshape(3,3)

	dist_coefs_str = file_lines[5:]
	dist_coefs_str_lines = [line.strip() for line in dist_coefs_str]
	dist_coefs = []
	for x in dist_coefs_str_lines:
		x = x.replace('[','')
		x = x.replace(']','')
		x_t = np.fromstring(x,sep=' ')
		dist_coefs = np.append(dist_coefs, x_t)
	#print dist_coefs.shape
	
def getAffineTransform(coord1, coord2):
		"""
		Given 2 sets of corresponding coordinates,
		find the affine matrix transform between them.

		TODO: Rewrite this function to take in an arbitrary number of coordinates and
		find the transform without using cv2 functions
		"""
		num = coord1.size/2
		# print num
		# affine_array = np.zeros((6,1),np.float32)
		A = []
		for x in xrange(num):
			A = np.append(A,[coord1[x][0],coord1[x][1],1,0,0,0,0,0,0,coord1[x][0],coord1[x][1],1])
		A = A.reshape(2*num,6)
		A_p_inv = np.linalg.inv(np.matmul(np.transpose(A),A))
		aff = np.matmul(np.matmul(A_p_inv,np.transpose(A)),np.transpose(coord2.ravel()))
		aff = aff.reshape(2,3)
		return aff
"""main function"""
if __name__ == '__main__':
	#writeConfg()
    #loadCameraCalibration()
    #print np.zeros((5,2),int)
    coord1 = np.array([[50,50],[200,50],[50,200],[20,100]],int)
    coord2 = np.array([[10,100],[200,50],[100,250],[150,200]],int)
    #print coord1[0:3].astype(np.float32)
    #print coord2[0:3].astype(np.float32)
    #pts1 = coord1[0:3].astype(np.float32)
    #pts2 = coord2[0:3].astype(np.float32)
    #print(cv2.getAffineTransform(pts1,pts2))
    #m = cv2.getAffineTransform(pts1,pts2)
    #m = m.ravel()
    #m = np.append(m,[0,0,1])
    #m = m.reshape(3,3)
    #print m
    #print np.matmul(m,np.transpose([50,50,1]))
    # num = coord1.size/2
    # A = []
    # for x in xrange(num):
    # 	A = np.append(A,[coord1[x][0],coord1[x][1],1,0,0,0,0,0,0,coord1[x][0],coord1[x][1],1])
    # A = A.reshape(2*num,6)
    # print A
    # A_t = np.linalg.inv(np.matmul(np.transpose(A),A))
    # aff = np.matmul(np.matmul(A_t,np.transpose(A)),np.transpose(coord2.ravel()))
    # aff = aff.reshape(2,3)
    # print aff
    affine = getAffineTransform(coord1,coord2)
    print affine

    affine = np.block([affine, np.zeros((2,1))])
    print affine


