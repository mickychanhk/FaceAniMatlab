function [alpha_exp] = FittingExpression(pt2d, pt3d_shape, keypoints1, R, t, s, w, sigma, beta)


s3d = s * R * pt3d_shape;
s2d = s3d([1,2],:);
t2d = repmat(t([1,2]), 1, size(pt2d,2));

%% Firstly the expression component
m = size(pt2d,2);
n = size(w,2);

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
equationRight = w2d' * (pt2d(:) - s2d(:) - t2d(:));
alpha_exp = (equationLeft \ equationRight)';

alpha_exp = alpha_exp';
end

