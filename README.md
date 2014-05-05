Object-oriented-CSL-random-forest
=================================

A C++ implementation of CSL- random forest

A Supervised learning algorithm which can classify multi-class problems. Brief Description: A random forest classifcation algorithm which used Kmeans ro perform data splits at each node ( the splits are unsupervised , and donot use labels)

Data format: H (n*m) input matrix with n points and m features . 

Labels (n*1) - Labels to corresponding data files.

Creating Train and Test datasets for multiple runs. You can use the "supervised_bagging" function of this purpose. (Let me know if any difficulty) 

TrainDMapIndices - Indices of randomly shuffled training points. 

TrainLabels - Corresponding training labels. 

TestDMapIndices - Indices of randomly shuffled testing points. 

TestLabels - Corresponding testing labels.  

Other parameters in the file can be changed according to the need.

Please let me know before any major changes.

