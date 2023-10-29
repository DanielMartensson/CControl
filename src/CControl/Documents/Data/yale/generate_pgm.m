function generate_pgm()
  % Get current working directory
  sub_folder_path = pwd;

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
