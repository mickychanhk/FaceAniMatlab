function [alpha_exp] = FittingExpression(pt2d, pt3d_shape, keypoints1, R, t, s, w, sigma, beta)


s3d = s * R * pt3d_shape;
s2d = s3d([1,2],:);
t2d = repmat(t([1,2]), 1, size(pt2d,2));

%% Firstly the expression component
m = size(pt2d,2);
n = size(w,2);

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
equationRight = w2d' * (pt2d(:) - s2d(:) - t2d(:));
alpha_exp = (equationLeft \ equationRight)';

alpha_exp = alpha_exp';
end

