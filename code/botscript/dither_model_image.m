dir1='../OpenGLobjloader/badmodel';
model3D=dir(dir1);
nmodel=numel(model3D);
for indx=1:nmodel
%     disp(model3D(indx).name);
    modelname=sprintf('%s/%s',dir1,model3D(indx).name);
    
    if isdir(modelname) && length(model3D(indx).name)>2
         content=dir(modelname);
         nimg=numel(content);
         for imIndx=1:nimg
%              disp(modelname);
            
            if length(content(imIndx).name)>2
                imgname=sprintf('%s/%s',modelname,content(imIndx).name);
                [path,name,ext]=fileparts(imgname);
                if strcmp(ext,'.bmp')
%                     disp(imgname);
                    I=imread(imgname);
                    dithImName=sprintf('%s/%s_d%s',path,name,ext);
                    I2=orderedDith(double(I),2);
                    I2=uint8((I2-min(I2(:)))/(max(I2(:))-min(I2(:)))*255);
                    imwrite(I2,dithImName,'bmp');
%                     disp(dithImName);

                end
            end
         end
    end
    
   
end
