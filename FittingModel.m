function [f, phi, gamma, theta, t3d, alpha, alpha_exp] = FittingModel(pt2d, mu, w, sigma, w_exp, sigma_exp, tri, parallel, keypoints, img)
%   fit the 3DMM with the 68 landmarks and return the pose, shape and
%   expression parameters
%   
%   Paras: 
%   @pt2d      : The 68 landmarks.
%   @mu        : The mean shape of 3DMM
%   @w         : The PCA axes of 3DMM shape
%   @sigma     : The PCA standard variations of 3DMM shape
%   @w_exp     : The PCA axes of 3DMM expression
%   @sigma_exp : The PCA standard variations of 3DMM expression
%   @parallel  : The parallel on the face model see section 2.2
%   @keypoints : the 68 landmarks make up on the 3D model, which has the
%                same semantic positions as pt2d
%   @img       : the input image
%
%   Outputs:
%   ==========
%   @f         : The scale parameter of pose
%   @phi       : The pitch angle of pose
%   @gamma     : The yaw angle of pose
%   @theta     : The roll angle of pose
%   @t3d       : The translation of pose
%   @alpha     : The shape paramters
%   @alpha_exp : The expression paramters
%
%
%   ==========
%   The Code is created based on the method described in the following paper:
%   [1] "High-Fidelity Pose and Expression Normalization for Face Recognition in the Wild", 
%        Xiangyu Zhu, Zhen Lei, Junjie Yan, Dong Yi and Stan Z. Li
%        IEEE Conference on Computer Vision and Pattern Recognition (CVPR),
%        2015
%
%   The code and the algorithm are for non-comercial use only.
%
%  
%   Author: Xiangyu Zhu (frank.xy.zhu@gmail.com)
%   Date  : 11/04/2015
%   Copyright 2015, Center for Biometrics and Security Research & 
%   National Laboratory of Pattern Recognition,
%   Institute of Automation, Chinese Academy of Sciences,
% 


[height, width, nChannels] = size(img);

PI = 3.1415926;

LeftVis =  [1:9, 18:48, 49, 61 65 55]; 
RightVis = [9:17, 18:48, 49, 61 65 55];

% Firstly Pose Coarse Estimation, set valid keypoints
vertex = reshape(mu, 3, length(mu)/3);
pt3d = vertex(:, keypoints); 

%% Coarse Pose Estimate
[phil, gammal, thetal, t3d, f] = FittingPose(pt2d(:, LeftVis), pt3d(:, LeftVis));
[phir, gammar, thetar, t3d, f] = FittingPose(pt2d(:, RightVis), pt3d(:, RightVis));
[temp, max_ind] = max([abs(gammal), abs(gammar)]);
if(max_ind == 1)
    phi = phil;
    gamma = gammal;
    theta = thetal;
else
    phi = phir;
    gamma = gammar;
    theta = thetar;
end
R = RotationMatrix(phi, gamma, theta);

valid_key = [1:68];
valid_key1 = [3 * valid_key-2; 3*valid_key-1; 3*valid_key];
valid_key1 = valid_key1(:);
    

%% modify keypoints in each interation
% normal algorithm

% shape and expression parameters
alpha = zeros(size(w,2), 1);
alpha_exp = zeros(size(w_exp,2), 1);

iteration = 0;
maxiteration = 4;

%% Firstly pose and expression
while(true)
	if(iteration > maxiteration)
        break;
    end
    iteration = iteration + 1;
    
    %% 1. Update keypoints
    if(gamma < 0)
        modify_ind = [10:17];
    else
        modify_ind = [1:8];
    end
    [keypoints_cur] = KeypointsWithPose(phi, gamma, theta, vertex, tri, parallel, keypoints, modify_ind);
    keypoints_cur1 = [3*keypoints_cur-2; 3*keypoints_cur-1; 3*keypoints_cur];
    keypoints_cur1 = keypoints_cur1(:);
    
    % truncate model with keypoint index
    mu_key = mu(keypoints_cur1); 
    w_key = w(keypoints_cur1, :); 
    w_exp_key = w_exp(keypoints_cur1, :);

    express3d_key = w_exp_key * alpha_exp;
    express3d_key = reshape(express3d_key, 3, length(express3d_key)/3);

    shape3d_key = mu_key + w_key * alpha;
    shape3d_key = reshape(shape3d_key, 3, length(shape3d_key)/3);
    vertex_key = shape3d_key + express3d_key;

%     pt3d1 = f * R * vertex_key + repmat(t3d, 1, size(vertex_key,2));
%     pt3d1(2,:) = height + 1 - pt3d1(2,:);
%     pt2d1 = pt2d;
%     pt2d1(2,:) = height + 1 - pt2d1(2,:);
%     imshow(img);
%     hold on;
%     plot(pt2d1(1,:), pt2d1(2,:), 'b.');
%     plot(pt3d1(1,:), pt3d1(2,:), 'r.');
%     hold off;
    
    %% 2. Pose Estimate  
    pose_fitting_ind = [1:17, 28:36, 37:48, 49, 61 65 55];
    pt3d = vertex_key;
    [phi, gamma, theta, t3d, f] = FittingPose(pt2d(:, pose_fitting_ind), pt3d(:, pose_fitting_ind));
    R = RotationMatrix(phi, gamma, theta);
    
    %% 2.expression fitting
    % update shape3d 
    % Usually we can't fit expression accuratly, which always lead to over
    % normalization, thus we weaken expression coefficients
    beta_exp = 1000;
    [alpha_exp] = FittingExpression(pt2d(:, valid_key), shape3d_key(:, valid_key), [1: length(valid_key1)], R, t3d, f, w_exp_key(valid_key1, :), sigma_exp, beta_exp);
    express3d_key = w_exp_key * alpha_exp;
    express3d_key = reshape(express3d_key, 3, length(express3d_key)/3);
     
    %% 3.shape fitting
    beta_shape = 3000;
    [alpha] = FittingShape(pt2d(:, valid_key), express3d_key(:, valid_key), [1: length(valid_key1)], R, t3d, f, mu_key(valid_key1), w_key(valid_key1,:), sigma, beta_shape);
end


end



