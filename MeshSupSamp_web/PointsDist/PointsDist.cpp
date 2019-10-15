#include <Windows.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <mex.h>
#include <matrix.h>
#include "../vec3.h"
#include "../nanoflann.h"

#include <stdio.h>


// compute distances for every point in A to closet point in B
void PointsDist(const PointCloud& pointcloudA, const PointCloud& pointcloudB, const vec3d& minX, const vec3d& maxX, double maxDist, double* dists)
{
	ASSERT(!pointcloudA.empty() && !pointcloudB.empty());
	ASSERT(maxDist>0);

	struct PointsAdaptor {
		typedef vec3d::Type Scalar;
		typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<Scalar, PointsAdaptor>, PointsAdaptor, 3> KDTreeAdaptor;
		typedef KDTreeAdaptor::BoundingBox BBox;
		const PointCloud& points;
		const BBox box;
		inline size_t kdtree_get_point_count() const { return points.size(); }
		inline Scalar kdtree_get_pt(const size_t idx, size_t dim) const { return points[idx][(int)dim]; }
		inline bool kdtree_get_bbox(BBox& bb) const { /*bb = box; return true;*/ return false; }
	};
	const PointsAdaptor::BBox box{minX.x,maxX.x,minX.y,maxX.y,minX.z,maxX.z};
	const PointsAdaptor adaptor{pointcloudB, box};
	PointsAdaptor::KDTreeAdaptor index(3, adaptor);
	index.buildIndex();
	const size_t knn(1);
	#ifdef _USE_OPENMP
	const unsigned maxThreads(omp_get_max_threads());
	#pragma omp parallel for num_threads(maxThreads)
	for (int64_t _idx=0; _idx<(int64_t)pointcloudA.size(); ++_idx) {
		const size_t i((size_t)_idx);
	#else
	for (size_t i=0; i<pointcloudA.size(); ++i) {
	#endif
		double& distSq = dists[i];
		const vec3d& X = pointcloudA[i];
		if (X.x  < minX.x || X.y  < minX.y || X.z  < minX.z ||
			X.x >= maxX.x || X.y >= maxX.y || X.z >= maxX.z)
		{
			distSq = maxDist;
			continue;
		}
		// find point nearest neighbors
		size_t idx;
		if (index.knnSearch(X.ptr(), knn, &idx, &distSq) > 0)
			distSq = std::min(sqrt(distSq), maxDist);
		else
			distSq = maxDist;
	}
}

void mexFunction(int nOut, mxArray *Out[], int nIn, const mxArray *In[])
{
	if(nIn < 3 || mxGetM(In[0])!=3 || mxGetM(In[1])!=3)
		mexErrMsgTxt("Qm and Pm should have 3 rows") ;

	PointCloud pointcloudA; {
	size_t nQ=mxGetN(In[0]);
	double* pQ=mxGetPr(In[0]);
	pointcloudA.reserve(nQ);
	for(size_t cQ=0;cQ<nQ;cQ++) {
		double x=*pQ;
		++pQ;
		double y=*pQ;
		++pQ;
		double z=*pQ;
		++pQ;

		pointcloudA.emplace_back(x,y,z);
	}
	}

	PointCloud pointcloudB; {
	size_t nQ=mxGetN(In[1]);
	double* pQ=mxGetPr(In[1]);
	pointcloudB.reserve(nQ);
	for(size_t cQ=0;cQ<nQ;cQ++) {
		double x=*pQ;
		++pQ;
		double y=*pQ;
		++pQ;
		double z=*pQ;
		++pQ;

		pointcloudB.emplace_back(x,y,z);
	}
	}

	vec3d minX(-FLT_MAX), maxX(FLT_MAX);
	if (nIn > 3) {
		{
		const double* pQ=mxGetPr(In[2]);
		minX = vec3d(pQ[0], pQ[1], pQ[2]);
		}
		{
		const double* pQ=mxGetPr(In[3]);
		maxX = vec3d(pQ[0], pQ[1], pQ[2]);
		}
	}

	const double maxDist=*mxGetPr(In[nIn > 3 ? 4 : 2]);

	#if 0
	size_t n;
	FILE* fd=fopen("test.dat", "wb");
	n = pointcloudA.size();
	fwrite(&n, sizeof(size_t), 1, fd);
	fwrite(pointcloudA.data(), sizeof(double), n, fd);
	n = pointcloudB.size();
	fwrite(&n, sizeof(size_t), 1, fd);
	fwrite(pointcloudB.data(), sizeof(double), n, fd);
	fwrite(minX.ptr(), sizeof(double), 3, fd);
	fwrite(maxX.ptr(), sizeof(double), 3, fd);
	fwrite(&maxDist, sizeof(double), 1, fd);
	fclose(fd);
	#endif

	Out[0] = mxCreateDoubleMatrix(1, pointcloudA.size(), mxREAL);
	double* pOut=mxGetDoubles(Out[0]);
	PointsDist(pointcloudA, pointcloudB, minX, maxX, maxDist, pOut);
}

#if 0
void main()
{
	PointCloud pointcloudA;
	PointCloud pointcloudB;
	vec3d minX, maxX;
	double maxDist;
	size_t n;
	FILE* fd=fopen("test.dat", "rb");
	fread(&n, sizeof(size_t), 1, fd);
	pointcloudA.resize(n);
	fread(pointcloudA.data(), sizeof(double), n, fd);
	fread(&n, sizeof(size_t), 1, fd);
	pointcloudB.resize(n);
	fread(pointcloudB.data(), sizeof(double), n, fd);
	fread(minX.ptr(), sizeof(double), 3, fd);
	fread(maxX.ptr(), sizeof(double), 3, fd);
	fread(&maxDist, sizeof(double), 1, fd);
	fclose(fd);

	std::vector<double> dists;
	PointsDist(pointcloudA, pointcloudB, minX, maxX, maxDist, dists);
}
#endif
