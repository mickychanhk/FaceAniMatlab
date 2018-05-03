function [alpha] = FittingShape(pt2d, pt3d_express, keypoints1, R, t, s, mu, w, sigma, beta)
% Initialize Shape with Keypoint
% @input pt3d: Keypoint on the modal;
% @input pt2d: Keypoint on the image
% @input keypoints1: Keypoint index on the modal
% @input R t s: Pose parameter
% @input beta: Regularization parameter;
% @input sigma: Shpae's PCA parameter sigma
% @output alpha: Shape's PCA paramter

beta = s *  beta;

m = size(pt2d,2);
n = size(w,2);

s3d = mu(keypoints1);
s3d = reshape(s3d, 3, length(s3d)/3);
s3d = s * R * s3d;
s2d = s3d([1,2],:);

e3d = s * R * pt3d_express;
e2d = e3d([1,2], :);

t2d = repmat(t([1,2]), 1, size(pt2d,2));

w2d = zeros(2 * m, n);
for i = 1 : n 
    tempdata = reshape(w(keypoints1, i), 3, m); %第i个特征脸的关键点3D坐标
    tempdata2d =  s * R * tempdata; %投影到2D上
    tempdata2d = tempdata2d(1 : 2, :);
    w2d(:, i) = tempdata2d(:); %特征脸上的关键点投影到2D的坐标 w2d = keypoint(T * w)= T * keypoint(w) (T为仿射矩阵，w为特征脸) 
end


% optimize the equation
%要求目标3D model的关键点经过投影后与2D图像关键点重合，作为形状约束
%公式为：优化公式为：||x - T(w * alpha + mu)|| + lambda * alpha' * C * alpha
%求极小值得：(w'T'Tw + lambda*C) * alpha = w'T'x - w'T'T*mu 其中w2d = wT 
equationLeft = w2d' * w2d + beta * diag(1 ./ (sigma .* sigma));
equationRight = w2d' * (pt2d(:) - s2d(:) - e2d(:) - t2d(:));
alpha = equationLeft \ equationRight;

end

