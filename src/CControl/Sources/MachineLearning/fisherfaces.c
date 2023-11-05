/*
 * fisherfaces.c
 *
 *  Created on: 27 mars 2023
 *      Author: Daniel Mårtensson
 */

#include "../../Headers/functions.h"

/*
 * This is an algorithm that was created in the year 1997 by Peter N. Belhumeur, Joao P. Hespanha, and David J. Kriegman.
 * But I have made som improvements to this algorithm.
 * Instead of regular PCA, I have added Kernel PCA.
 * Instead of K-nn, I have included SVM instead.
 * This algorithm has a very high accuracy for classifying images e.g faces or other types of objects.
 * Notice that this algorithm can only classify one object at the time. It has no localization feature.
 * Pros:
 *  - Classify general objects
 *  - Fast
 * Cons:
 *  - Does not offer localization
 *  - Only one object at the time
 */
DATA_COLLECT* fisherfaces(DATA_SETTINGS* settings) {
	/* Header */
	printf("\t\t\t\tFisherfaces\n");
	
	/* Collect data */
	printf("1: Collecting data. Reading the .pgm files in row-major. PGM format P2 or P5 format.\n");
    settings->data_settings_choice = DATA_SETTINGS_CHOICE_FISHERFACES;
	DATA_COLLECT* data_collect = imcollect(settings);

    /* Extract */
    DATA_SETTINGS_FISHERFACES* fisherfaces_settings = &settings->data_settings_fisherfaces;
    MODEL* fisherfaces_models = &data_collect->fisherfaces_models;

	/* Remove outliers */
	if (fisherfaces_settings->remove_outliers) {
		printf("\tOutliers removed: %i\n", cluster_filter(data_collect->input, data_collect->input_row, data_collect->input_column, fisherfaces_settings->epsilon, fisherfaces_settings->min_pts));
	}

	/*
	 * Parameters for KPCA:
	 * components_pca is a user defined tuning parameter
	 * Example:
	 * const size_t components_pca = 100;
	 * float kernel_parameters[2] = { 0.0000001f, 3.1 };
	 * KERNEL_METHOD kernel_method = KERNEL_METHOD_RBF;
	 */
	printf("2: Do Kernel Principal Component Analysis for creating a linear projection for nonlinear data.\n");
	float* Wpca = (float*)malloc(data_collect->input_row * fisherfaces_settings->components_pca * sizeof(float));
	float* Ppca = (float*)malloc(fisherfaces_settings->components_pca * data_collect->input_column * sizeof(float));
	kpca(data_collect->input, Wpca, Ppca, fisherfaces_settings->components_pca, data_collect->input_row, data_collect->input_column, fisherfaces_settings->kernel_parameters, fisherfaces_settings->kernel_method);

	/*
	 * Parametets for LDA:
	 * components_lda must be classes - 1 because we want to avoid zero eigenvalues
	 */
	printf("3: Do Linear Discriminant Analysis for creating a linear projection for separation of class data.\n");
	const size_t components_lda = data_collect->classes_original - 1;
	float* Wlda = (float*)malloc(fisherfaces_settings->components_pca * components_lda * sizeof(float));
	float* Plda = (float*)malloc(components_lda * data_collect->input_column * sizeof(float));
	lda(Ppca, data_collect->class_id_original, Wlda, Plda, components_lda, fisherfaces_settings->components_pca, data_collect->input_column);

	/* Multiply W = Wlda'*Wpca' */
	printf("4: Combine Kernel Principal Component Analysis projection with Linear Discriminant projection.\n");
	float* W = (float*)malloc(components_lda * data_collect->input_row * sizeof(float));
	tran(Wlda, fisherfaces_settings->components_pca, components_lda);
	tran(Wpca, data_collect->input_row, fisherfaces_settings->components_pca);
	mul(Wlda, Wpca, W, components_lda, fisherfaces_settings->components_pca, data_collect->input_row);

	/* Free some matrices that are not needed any longer */
	free(Wpca);
	free(Ppca);
	free(Wlda);
	free(Plda);

	/* Find the total projection */
	printf("5: Find the total projection of the nonlinear data.\n");
	float* P = (float*)malloc(components_lda * data_collect->input_column * sizeof(float));
	mul(W, data_collect->input, P, components_lda, data_collect->input_row, data_collect->input_column);

	/*
	 * Train Neural Network model of the total projection
	 * C is a tuning parameter
	 * lambda is a tuning parameter
	 * Important to turn P into transpose because we are going to use that as vectors ontop on each other
	 * Example:
	 * float C = 1;
	 * float lambda = 2.5;
	 */
	printf("6: Create a Neural Network for a linear model that can handle nonlinear data.\n");
	tran(P, components_lda, data_collect->input_column);
	float* accuracy = (float*)malloc(data_collect->classes_original * sizeof(float));
	bool* status = (bool*)malloc(data_collect->classes_original * sizeof(bool));
	float* weight = (float*)malloc(data_collect->classes_original * components_lda * sizeof(float));
    fisherfaces_models->model_b[0] = (float*)malloc(data_collect->classes_original * sizeof(float));
	nn_train(P, data_collect->class_id_original, weight, fisherfaces_models->model_b[0], status, accuracy, data_collect->input_column, components_lda, data_collect->classes_original, fisherfaces_settings->C, fisherfaces_settings->lambda);

	/* Free */
	free(status);
	free(P);
	free(accuracy);

	/*
	 * Multiply model_w = weight * W
	 * y = model_w*imagevector + model_b is our model.
	 * The index of the largest value of vector y is the class ID of imagevector
	 */
	printf("7: Creating the model for nonlinear data.\n");
    fisherfaces_models->model_w[0] = (float*)malloc(data_collect->classes_original * data_collect->input_row * sizeof(float));
	mul(weight, W, fisherfaces_models->model_w[0], data_collect->classes_original, components_lda, data_collect->input_row);

	/* Free */
	free(W);
	free(weight);

	/* Save the row and column parameters */
    fisherfaces_models->model_row[0] = data_collect->classes_original;
    fisherfaces_models->model_column[0] = data_collect->input_row;
    fisherfaces_models->activation_function[0] = ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX;
    fisherfaces_models->total_models = 1;

	/* Check the accuracy of the model */
	tran(data_collect->input, data_collect->input_row, data_collect->input_column);
    float* y = (float*)malloc(data_collect->input_column * data_collect->classes_original * sizeof(float));
	nn_eval(fisherfaces_models->model_w[0], fisherfaces_models->model_b[0], data_collect->input, y, data_collect->class_id_original, data_collect->classes_original, data_collect->input_row, data_collect->input_column, ACTIVATION_FUNCTION_HIGHEST_VALUE_INDEX);
    free(y);

	/* Save wW and b as a function */
	printf("8: Saving the model to a .h file.\n");
	if (fisherfaces_settings->save_model) {
        char model_path[260];
        char model_name[100];
        char model_name_h[100];
        char model_name_text[100];
        printf("Enter a model name: ");
        scanf("%s", model_name);
        uint8_t i;
        for (i = 0; i < fisherfaces_models->total_models; i++) {
            sprintf(model_name_h, "%s_%i.h", model_name, i);
            sprintf(model_name_text, "%s_%i", model_name, i);
            concatenate_paths(model_path, fisherfaces_settings->folder_path, model_name_h);
            nn_save(fisherfaces_models->model_w[i], fisherfaces_models->model_b[i], fisherfaces_models->activation_function[i], model_path, model_name_text, fisherfaces_models->model_row[i], fisherfaces_models->model_column[i]);
        }
	}
	else {
		printf("\tNo...\n");
	}

	/* End */
	printf("9: Everything is done...\n");

	/* Return data */
	return data_collect;
}

/* 
GNU octave code
% Fisherfaces
% This algorithm can create a neural network model that can classify images or regular data
% This algorithm is using:
% Pooling -> Kernel Principal Component Analysis -> Linear Discriminant Analysis -> Support Vector Machine -> Activation function
% You will recieve a model inside fisherfaces_model.mat file
% It contains
% - Weight matrix model_W [m*n]
% - Bias vector model_b [m]
% - Activation function handle
% This Fisherfaces will give you the neural network model, and all you have to do is to multiply a vector image_vector [n] to get the class ID:
% y = activation_function(model_w*image_vector + model_b)
% Where y is the class ID of image_vector
% Example 1: mi.fisherfaces();
% Author: Daniel Mårtensson, Augusti 18 2023

function fisherfaces()
  % Salut
  disp('Welcome to Fisherfaces.')
  disp('This algorithm can classify images or raw data from .pgm files in the formats P2 and P5.')
  disp('Notice that it exist an equivalent ANSI C code for this inside CControl');

  % Ask the user what it want to do
  choice = input("What do you want to do?\n1. Collect data\n2. Train projection matrix\n3. Generate PGM data from images\n4. Check pooling\n5. Train neural network model\n6. Remove outliers from collected data\nEnter choice number: ");

  % Switch statement for the choices
  switch(choice)
  case 1
    fisherfaces_collect_data();
  case 2
    fisherfaces_train_projection_matrix();
  case 3
    fisherfaces_generate_pgm_data();
  case 4
    fisherfaces_check_pooling();
  case 5
    fisherfaces_train_neural_network_model();
  case 6
    fisherfaces_remove_outliers();
  otherwise
    disp('Unknown choice - Exit')
  end
end

function fisherfaces_remove_outliers()
  disp('Loading fisherfaces_data.mat');
  load('fisherfaces_data.mat', 'images', 'class_id');
  disp('Done');

  % Use DBSCAN to detect outliers
  disp('Remove outliers with DBscan');
  epsilon = input('Give the minimum epsilon for your data: ');
  min_pts = input('Give the minimum points for your data: ');
  idx = mi.dbscan(images, epsilon, min_pts);

  % Tell how many outliers it was
  choice = 2;
  while(choice == 2)
    amount_of_outliers = length(find(idx == 0));
    total_clusters = max(idx);
    choice = input(sprintf('Total outliers detected was %i. Total clusters of DBscan was %i. What do you want to do next?\n1. Remove them\n2. Change DBscan settings\nEnter choice number: ', amount_of_outliers, total_clusters));
    switch(choice)
      case 1
        % Outliers are at idx
        images_clean = images(idx ~= 0, :);

        % Find the mean of images_clean
        mu = mean(images_clean);

        % Replace all the outliers with mu
        images(idx == 0, :) = repmat(mu, amount_of_outliers, 1);
      case 2
        epsilon = input('Give the minimum epsilon for your data: ');
        min_pts = input('Give the minimum points for your data: ');
        idx = mi.dbscan(images, epsilon, min_pts);
      otherwise
        disp('Quit. Did not save anything new. Original data remains');
        return;
    end
  end

  % Save
  disp('Saving fisherfaces_data.mat');
  save('fisherfaces_data.mat', 'images', 'class_id');
  disp('Done');
end

function fisherfaces_train_neural_network_model()
  % Load data
  disp('Load fisherfaces_projection.mat');
  load('fisherfaces_projection.mat', 'P', 'W');
  disp('Done');

  % Load data
  disp('Loading fisherfaces_data.mat');
  load('fisherfaces_data.mat', 'images', 'class_id');
  disp('Done');

  % Ask the user about parameters
  disp('Train a neural network with Support Vector Machine');
  C = input('What type of C value do you want for SVM: ');
  lambda = input('What type of lambda(regularization) value do you want for SVM: ');

  % Create neural network - Turn P into transpose so it will work with SVM
  [weight, bias, activation_function] = mi.nn(P', class_id, C, lambda);

  % Create models
  model_w = weight*W;
  model_b = bias;

  % Check accuracy
  X = model_w*images + model_b;
  classes = length(class_id);
  score = 0;
  for i = 1:classes
    if(class_id(i) == activation_function(X(:, i)))
      score = score + 1;
    end
  end

  % Print status
  fprintf('The accuracy of this model is: %i\n', score/classes*100);

  % Now we have our model. Compute the ID
  disp('Saving model_w, model_b, and activation_function inside fisherfaces_model.mat');
  save('fisherfaces_model.mat', 'model_w', 'model_b', 'activation_function');
  disp('Done');
end

function fisherfaces_train_projection_matrix()
  % Load data
  disp('Loading fisherfaces_data.mat');
  load('fisherfaces_data.mat', 'images', 'class_id');
  disp('Done');

  % Ask the user what type of kernel type should be used
  kenel_type_id = input("What kernel type do you want to use?\n1. Linear\n2. Polynomial\n3. Gaussian\n4. Exponential\n5. Sigmoid\n6. RBF\nEnter choice number: ");
  switch(kenel_type_id)
  case 1
    kernel_type = 'linear';
    kernel_parameters = '';
  case 2
    kernel_type = 'polynomial';
    kernel_parameters = input("Enter degree value: ");
  case 3
    kernel_type = 'gaussian';
    kernel_parameters = input("Enter sigma value: ");
  case 4
    kernel_type = 'exponential';
     kernel_parameters = input("Enter sigma value: ");
  case 5
    kernel_type = 'sigmoid';
    kernel_parameters(1) = input("Enter alpha value: ");
    kernel_parameters(2) = input("Enter beta value: ");
  case 6
    kernel_type = 'rbf';
    kernel_parameters = input("Enter gamma value: ");
  otherwise
    error('Unknown kernel type');
  end

  % Get size of images
  [m, n] = size(images);

  % Get the amount of classes
  classes = max(class_id);

  % Reduce dimmension of images
  cpca = input('How many dimensions for KPCA algorithm: ');

  % Do KPCA
  [Ppca, Wpca] = mi.kpca(images, cpca, kernel_type, kernel_parameters);

  % Reduce dim(Ppca) to the amount of classes - 1
  clda = classes - 1;

  % Do LDA
  [Plda, Wlda] = mi.lda(Ppca, class_id, clda);

  % Get the projection matrix
  W = Wlda'*Wpca';
  P = W*images;

  % Save the projection matrix
  disp('Saving fisherfaces_projection.mat');
  save('fisherfaces_projection.mat', 'W', 'P');
  disp('Done');
end

function fisherfaces_check_pooling()
  % Ask the user of a file path to an image
  file_path = input("Enter the file path to an image: ", 's');

  % Load imwrite (if using Octave)
  if ~isempty(ver('Octave'))
   pkg load image
  end

  % Read the image
  image = imread(file_path);

  % Turn the image to gray scale if needed
  [~, ~, l] = size(image);
  if(l > 1)
    image = rgb2gray(image);
  end

  % Show the original image
  imshow(image);
  title(sprintf('Original image with size %ix%i', size(image)))

  % Ask the user about pooling
  happy = 0;
  fig = 0;
  while(happy == 0)
    pooling_method = input("What pooling method do you want to use?\n1. Max pooling\n2. Average pooling\n3. Shape pooling\nEnter choice number: ");
    p = input("What pooling size do you want to use? ");

    % Generate pooling
    P = mi.pooling(double(image), pooling_method, p);

    % Show the image
    if(fig > 0)
      figure(fig);
      imshow(uint8(P));
    else
      fig = figure;
      imshow(uint8(P));
    end
    title(sprintf('Pooled image with size %ix%i', size(P)))

    % Ask the user if it's done
    happy = input("Are you happy with the result? 1 = Yes, 0 = No: ");
  end

  % Salut
  disp('Done...');
end

function fisherfaces_generate_pgm_data()
  % Ask the user where the images are
  sub_folder_path = input("Enter the folder path of the sub folders: ", 's');

  % Load imwrite (if using Octave)
  if ~isempty(ver('Octave'))
   pkg load image
  end

  % Scan the folder names
  sub_folder_names = dir(sub_folder_path);

  % Loop
  for i = 1:length(sub_folder_names)
    % Check if it's a directory
    if(sub_folder_names(i).isdir)
      % Check if the folder name is . or ..
      sub_folder_name = sub_folder_names(i).name;

      if(and(~strcmp(sub_folder_name, '.'), ~strcmp(sub_folder_name, '..')))
        % Combine sub_folder_name with sub_folder_path
        sub_file_folder_path = strcat(sub_folder_path, '\', sub_folder_name);

        % Scan the files
        file_names = dir(sub_file_folder_path);

        % Loop
        for j = 1:length(file_names)
          % Check if it's a file
          if(~file_names(j).isdir)
           % Ge the file name
            file_name = file_names(j).name;

            % Cannot be a .pgm file
            if(endsWith(file_name, '.pgm'))
              continue;
            end

            % Create the total file path
            total_file_path = strcat(sub_file_folder_path, '\', file_name);

            % Read the image
            image = imread(total_file_path);

            % Turn the image to gray scale if needed
            [~, ~, l] = size(image);
            if(l > 1)
              image = rgb2gray(image);
            end

            % Save the image as .pgm
            [folder, fileName, fileExt] = fileparts(total_file_path);
            total_file_path = strcat(folder, '\', fileName, '.pgm');
            fprintf('Creating image: %s\n', total_file_path);
            imwrite(image, total_file_path);

          end
        end
      end
    end
  end
  disp('Done');
end

function fisherfaces_collect_data()
  % Ask the user where the images are
  sub_folder_path = input("Enter the folder path of the sub folders: ", 's');

  % Ask the user if the user want pooling
  use_pooling = input("Do you want to use pooling? 1 = Yes, 0 = No: ");

  % If pooling is used
  if(use_pooling == 1)
    % Ask the user what pooling method sould be used
    pooling_method = input("What pooling method do you want to use?\n1. Max pooling\n2. Average pooling\n3. Shape pooling\nEnter choice number: ");
    p = input("What pooling size do you want to use? ");
  end

  % Scan the folder names
  sub_folder_names = dir(sub_folder_path);

  % Create the images holder
  images = [];
  class_id = [];
  counted_class_id = 0;

  % Loop
  for i = 1:length(sub_folder_names)
    % Check if it's a directory
    if(sub_folder_names(i).isdir)
      % Check if the folder name is . or ..
      sub_folder_name = sub_folder_names(i).name;

      if(and(~strcmp(sub_folder_name, '.'), ~strcmp(sub_folder_name, '..')))
        % Combine sub_folder_name with sub_folder_path
        sub_file_folder_path = strcat(sub_folder_path, '\', sub_folder_name);

        % Scan the files
        file_names = dir(sub_file_folder_path);

        % Count class id
        counted_class_id = counted_class_id + 1;

        % Loop
        for j = 1:length(file_names)
          % Check if it's a file
          if(~file_names(j).isdir)
           % Ge the file name
            file_name = file_names(j).name;

            % Must be a .pgm file
            if(~endsWith(file_name, '.pgm'))
              continue;
            end

            % Create the total file path
            total_file_path = strcat(sub_file_folder_path, '\', file_name);
            fprintf('Reading: %s\n', total_file_path);

            % Read the image
            image = imread(total_file_path);

            % Turn the image to gray scale if needed
            [~, ~, l] = size(image);
            if(l > 1)
              image = rgb2gray(image);
            end

            % Create image to double data type
            image = double(image);

            % Use pooling
            if(use_pooling == 1)
              image = mi.pooling(image, pooling_method, p);
            end

            % Read the images in row major
            image_vector = [];
            m = size(image, 1);
            for k = 1:m
              image_vector = [image_vector image(k, :)];
            end

            % Add the image vector transpose to images
            images = [images image_vector'];
            class_id = [class_id counted_class_id];
          end
        end
      end
    end
  end

  % Save the images and class_id
  disp('Saving fisherfaces_data.mat');
  save('fisherfaces_data.mat', 'images', 'class_id');
  disp('Done');
end
*/