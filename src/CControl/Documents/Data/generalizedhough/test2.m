pkg load image
X = rgb2gray(imread('face_train.jpg'));


[~, Y] = mi.sobel(X);
Y(Y > 150) = 0;

%Z =  mi.fast(Y, 10, 9);

imshow(uint8(Y));

%hold on

%scatter(Z(:, 1), Z(:, 2))
