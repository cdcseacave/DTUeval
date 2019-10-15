function eval_acc_compl = eval_ply(cSet, DataInName)
% cSet: model index
% DataInName: input '.ply' file name
% EvalName: save the evaluation into this '.mat' file
dst=0.2;    %Min dist between points when reducing
[dataPath,data3dnPath,resultsPath]=getPaths();
Data3dnInName=[data3dnPath 'run' sprintf('%d',cSet) '/' DataInName];
EvalName=[Data3dnInName '.mat'];

%load(EvalName,'BaseEval');
%BaseEval2Obj_web(BaseEval,'Surface', resultsPath);

addpath('MeshSupSamp_web\x64\Release');

tic
disp('Load Eval points')
Mesh = plyread(Data3dnInName);
Qdata=[Mesh.vertex.x Mesh.vertex.y Mesh.vertex.z]';
if(isfield(Mesh, 'face'))
    %upsample triangles
    Tri=cell2mat(Mesh.face.vertex_indices)';
    Qdata=MeshSupSamp(Qdata,Tri,dst);
end
toc

BaseEval=PointCompareMain(cSet,Qdata,dst,dataPath);
%save(EvalName,'BaseEval');

%eval_acc_compl = [mean(BaseEval.Ddata), median(BaseEval.Ddata), mean(BaseEval.Dstl), median(BaseEval.Dstl)]
%eval_acc_compl = [mean(BaseEval.Ddata(1,BaseEval.DataInMask)), median(BaseEval.Ddata .* BaseEval.DataInMask), mean(BaseEval.Dstl .* BaseEval.StlAbovePlane), median(BaseEval.Dstl .* BaseEval.StlAbovePlane)]
Ddata=BaseEval.Ddata(BaseEval.DataInMask~=0);
eval_acc_compl = [mean(Ddata), median(Ddata), mean(BaseEval.Dstl), median(BaseEval.Dstl)]
fid = fopen([Data3dnInName '.txt'], 'at');
fprintf(fid, '%f,%f,%f,%f\n', eval_acc_compl(1), eval_acc_compl(2), eval_acc_compl(3), eval_acc_compl(4));
fclose(fid);

% write obj-file of evaluation
%BaseEval2Obj_web(BaseEval,'Surface', resultsPath);
