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
    tempdata = reshape(w(keypoints1, i), 3, m); %��i���������Ĺؼ���3D����
    tempdata2d =  s * R * tempdata; %ͶӰ��2D��
    tempdata2d = tempdata2d(1 : 2, :);
    w2d(:, i) = tempdata2d(:); %�������ϵĹؼ���ͶӰ��2D������ w2d = keypoint(T * w)= T * keypoint(w) (TΪ�������wΪ������) 
end


% optimize the equation
%Ҫ��Ŀ��3D model�Ĺؼ��㾭��ͶӰ����2Dͼ��ؼ����غϣ���Ϊ��״Լ��
%��ʽΪ���Ż���ʽΪ��||x - T(w * alpha + mu)|| + lambda * alpha' * C * alpha
%��Сֵ�ã�(w'T'Tw + lambda*C) * alpha = w'T'x - w'T'T*mu ����w2d = wT 
equationLeft = w2d' * w2d + beta * diag(1 ./ (sigma .* sigma));
equationRight = w2d' * (pt2d(:) - s2d(:) - e2d(:) - t2d(:));
alpha = equationLeft \ equationRight;

end

