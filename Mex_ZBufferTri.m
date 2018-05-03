function [img, tri_ind] = Mex_ZBufferTri(projectedVertex, tri, texture_tri, img_src)
    projectedVertex = projectedVertex - 1;
    tri = tri - 1;
    [height, width, nChannels] = size(img_src);
    nver = size(projectedVertex,2);
    ntri = size(tri,2);
    [img, tri_ind] = Mex_ZBufferTriC(double(projectedVertex), double(tri), double(texture_tri), img_src, nver, ntri, width, height, nChannels);
    tri_ind = tri_ind + 1;
end