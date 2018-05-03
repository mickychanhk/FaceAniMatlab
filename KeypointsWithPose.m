function [keypoints_pose] = KeypointsWithPose(phi, gamma, theta, vertex, tri, isoline, keypoints, modify_ind, candidates)
    ProjectVertex = RotationMatrix(phi, gamma, 0) * vertex;
    ProjectVertex = ProjectVertex - repmat(min(ProjectVertex,[], 2), 1, size(ProjectVertex,2)) + 1;
    ProjectVertex = ProjectVertex/max(abs(ProjectVertex(:)));
    
    keypoints_pose = keypoints;
    
    % 1. get the keypoints needing modifying
%     if(gamma < 0)
%         modify_key = [10:17];
%     else
%         modify_key = [1:8];
%     end
    modify_key = modify_ind;
    
    % 2. get the contour line(等高线) of each modify key
%     contour_line = cell(length(modify_key), 1);
%     line_width = 0.05;
%     for i = 1:length(modify_key)
%         % find contour line(等高线)
%         pt = ProjectVertex(:, keypoints(modify_key(i)));
%         line_ind = find(abs(ProjectVertex(2,:) - pt(2)) < line_width);
%         line_ind = intersect(line_ind, find(face_bin==1));
%         contour_line{i} = line_ind;
%     end
    
    contour_line = isoline(modify_key);
   
    if(nargin >= 9)
        candidates = candidates(modify_key);
        for i = 1:length(modify_key)
            if(~isempty(candidates{i}))
                contour_line{i} = intersect(contour_line{i}, candidates{i});
            end
        end  
    end
    
    % 3. get the outest point on the contour line
    for i = 1:length(modify_key)
        if(gamma >= 0)
            [mmin, min_ind] = min(ProjectVertex(1, contour_line{i}));
            keypoints_pose(modify_key(i)) = contour_line{i}(min_ind);
        else
            [mmax, max_ind] = max(ProjectVertex(1, contour_line{i}));
            keypoints_pose(modify_key(i)) = contour_line{i}(max_ind);
        end
    end
    
%     t = zeros(size(ProjectVertex));
%     t(1,keypoints_pose)=1;
%     DrawTextureHead(ProjectVertex, tri, t);
%     DrawSolidHead(ProjectVertex, tri, ProjectVertex(:,keypoints_pose));
end

