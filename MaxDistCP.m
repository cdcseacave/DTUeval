function Dist = MaxDistCP(Qto,Qfrom,BB,MaxDist)

tic

% Dist=ones(1,size(Qfrom,2))*MaxDist;
% Range=floor((BB(2,:)-BB(1,:))/MaxDist);
% for x=0:Range(1)
%     for y=0:Range(2)
%         for z=0:Range(3)
%             Low=BB(1,:)+[x y z]*MaxDist;
%             High=min(Low+MaxDist, BB(2,:));
%             idxF=find(Qfrom(1,:)>=Low(1) & Qfrom(2,:)>=Low(2) & Qfrom(3,:)>=Low(3) &...
%                 Qfrom(1,:)<High(1) & Qfrom(2,:)<High(2) & Qfrom(3,:)<High(3));
%             SQfrom=Qfrom(:,idxF);
%             if(isempty(SQfrom))
%                 continue;
%             end
% 
%             Low=Low-MaxDist;
%             High=High+MaxDist;
%             idxT=find(Qto(1,:)>=Low(1) & Qto(2,:)>=Low(2) & Qto(3,:)>=Low(3) &...
%                 Qto(1,:)<High(1) & Qto(2,:)<High(2) & Qto(3,:)<High(3));
%             SQto=Qto(:,idxT);
%             
%             if(isempty(SQto))
%                 Dist(idxF)=MaxDist;
%             else
% %                KDstl=KDTreeSearcher(SQto');
% %                [~,SDist] = knnsearch(KDstl,SQfrom');
%                 SDist=PointsDist(SQfrom, SQto, Low, High, MaxDist);
%                 Dist(idxF)=SDist;
%             end
%         end
%     end
% end

% Low=BB(1,:);
% High=BB(2,:);
% 
% % idxF=find(Qfrom(1,:)>=Low(1) & Qfrom(2,:)>=Low(2) & Qfrom(3,:)>=Low(3) &...
% %     Qfrom(1,:)<High(1) & Qfrom(2,:)<High(2) & Qfrom(3,:)<High(3));
% % SQfrom=Qfrom(:,idxF);
% % KDstl=KDTreeSearcher(Qto');
% % [~,SDist] = knnsearch(KDstl,SQfrom');
% % Dist=ones(1,size(Qfrom,2))*MaxDist;
% % Dist(idxF)=SDist;
% 
% Dist=PointsDist(Qfrom, Qto, Low, High, MaxDist);

Dist=PointsDist(Qfrom, Qto, MaxDist);

toc
