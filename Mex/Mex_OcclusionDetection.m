function [visibility] = Mex_OcclusionDetection(vertex, tri, width, height, threshold, scale)
%OCCLUSIONQUERY Summary of this function goes here
%   Detailed explanation goes here
visibility = Mex_OcclusionQueryC(double(vertex * scale), double(tri), size(vertex,2), size(tri,2),  double(width * scale), double(height * scale), double(threshold * scale));

end

