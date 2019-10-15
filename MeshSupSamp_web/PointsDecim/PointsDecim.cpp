#include <Windows.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <mex.h>
#include <matrix.h>
#include "../vec3.h"
#include "../nanoflann.h"


// simplify the point set, in a stochastic manner,
// such that the minimum distance between points is the desired distance
void DecimatePointsDist(const PointCloud& pointcloud, float dist, PointIndexArr& indices)
{
	ASSERT(!pointcloud.empty());
	ASSERT(dist>0);

	// decide which points to remove
	std::vector<bool> valids(pointcloud.size(), true);
	{
	struct PointsAdaptor {
		typedef vec3d::Type Scalar;
		typedef nanoflann::KDTreeSingleIndexAdaptor<nanoflann::L2_Simple_Adaptor<Scalar, PointsAdaptor>, PointsAdaptor, 3> KDTreeAdaptor;
		typedef std::pair<size_t, Scalar> IndexDist;
		typedef std::vector<IndexDist> IndexDistArr;
		typedef KDTreeAdaptor::BoundingBox BBox;
		const PointCloud& points;
		inline size_t kdtree_get_point_count() const { return points.size(); }
		inline Scalar kdtree_get_pt(const size_t idx, size_t dim) const { return points[idx][(int)dim]; }
		inline bool kdtree_get_bbox(BBox& /*bb*/) const { return false; }
	};
	const PointsAdaptor adaptor{pointcloud};
	PointsAdaptor::KDTreeAdaptor index(3, adaptor);
	index.buildIndex();
	indices.resize(pointcloud.size());
	std::iota(indices.begin(), indices.end(), 0);
	std::random_device rd;   // non-deterministic generator
	std::mt19937 gen(rd());  // to seed mersenne twister.
	std::shuffle(indices.begin(), indices.end(), gen);
	const nanoflann::SearchParams searchParams(32, 0, false);
	const PointsAdaptor::Scalar radiusSq(dist*dist);
	PointsAdaptor::IndexDistArr indicesDists;
	#ifdef _USE_OPENMP
	const unsigned maxThreads(omp_get_max_threads());
	#pragma omp parallel for firstprivate(indicesDists) num_threads(maxThreads)
	for (int64_t _idx=0; _idx<(int64_t)pointcloud.size(); ++_idx) {
		const size_t i(indices[(size_t)_idx]);
	#else
	for (size_t i: indices) {
	#endif
		if (!valids[i])
			continue;
		const vec3d& point = pointcloud[i];
		// find point nearest neighbors
		index.radiusSearch(point.ptr(), radiusSq, indicesDists, searchParams);
		for (const PointsAdaptor::IndexDist& idxDist: indicesDists)
			if (idxDist.first != i)
				valids[idxDist.first] = false;
	}
	}

	// remove invalid points
	indices.clear();
	for (size_t i=0; i<valids.size(); ++i)
		if (valids[i])
			indices.push_back(i);
} // DecimatePointsDist

void mexFunction(int nOut, mxArray *Out[], int nIn, const mxArray *In[])
{
	size_t nQ=mxGetN(In[0]);
	if(mxGetM(In[0])!=3)
		mexErrMsgTxt("Qm should have 3 rows") ;

	const double dist=*mxGetPr(In[1]);

	double* pQ=mxGetPr(In[0]);
	PointCloud pointcloud;
	pointcloud.reserve(nQ);
	for(size_t cQ=0;cQ<nQ;cQ++)
	{
		double x=*pQ;
		++pQ;
		double y=*pQ;
		++pQ;
		double z=*pQ;
		++pQ;

		pointcloud.emplace_back(x,y,z);
	}


	PointIndexArr indices;
	DecimatePointsDist(pointcloud, dist, indices);
	mexPrintf("Point-cloud decimated (%g distance): %u/%u points (%.2f%%%%)\n", dist, indices.size(), pointcloud.size(), (100.f*indices.size())/pointcloud.size());

	Out[0] = mxCreateDoubleMatrix(3, indices.size(), mxREAL);
	double* pOut=mxGetDoubles(Out[0]);
	for (size_t i: indices)
	{
		const vec3d& X = pointcloud[i];
		*pOut=X.x;
		++pOut;
		*pOut=X.y;
		++pOut;
		*pOut=X.z;
		++pOut;
	}
}