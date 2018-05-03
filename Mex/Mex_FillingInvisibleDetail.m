function [result] = Mex_FillingInvisibleDetail(img, ref, mask, facemask, blend_width)
[height, width, nChannels] = size(img);
[result] = Mex_FillingInvisibleDetailC(double(img), double(ref), double(mask), double(facemask), double(width), double(height), double(nChannels), double(blend_width));


end

