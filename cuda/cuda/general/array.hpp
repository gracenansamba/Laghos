// Copyright (c) 2017, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-734707. All Rights
// reserved. See files LICENSE and NOTICE for details.
//
// This file is part of CEED, a collection of benchmarks, miniapps, software
// libraries and APIs for efficient high-order finite element and spectral
// element discretizations for exascale applications. For more information and
// source code availability see http://github.com/ceed.
//
// The CEED research is supported by the Exascale Computing Project 17-SC-20-SC,
// a collaborative effort of two U.S. Department of Energy organizations (Office
// of Science and the National Nuclear Security Administration) responsible for
// the planning and preparation of a capable exascale ecosystem, including
// software, applications, hardware, advanced system engineering and early
// testbed platforms, in support of the nation's exascale computing imperative.
#ifndef LAGHOS_CUDA_ARRAY
#define LAGHOS_CUDA_ARRAY

#include <cstddef>
#include <cassert>
#include "general/array.hpp"
#include "../general/malloc.hpp"
#include "../general/memcpy.hpp"

namespace mfem
{

template <class T, bool xyz = true> class CudaArray;

// Partial Specializations for xyz==TRUE *************************************
template <class T> class CudaArray<T,true> : public rmalloc<T>
{
private:
   T* data = NULL;
   int sz,d[4];
public:
   CudaArray():data(NULL),sz(0),d{0,0,0,0} {}
   CudaArray(const int x) {allocate(x);}
   CudaArray(const int x,const int y) {allocate(x,y);}
   CudaArray(const CudaArray<T,true> &r) {assert(false);}
   CudaArray& operator=(Array<T> &a)
   {
      rmemcpy::rHtoD(data,a.GetData(),a.Size()*sizeof(T));
      return *this;
   }
   ~CudaArray() {rmalloc<T>::operator delete (data);}
   inline int* dim() { return &d[0]; }
   inline T* ptr() { return data; }
   inline const T* GetData() const { return data; }
   inline const T* ptr() const { return data; }
   inline operator T* () { return data; }
   inline operator const T* () const { return data; }
   double operator* (const CudaArray& a) const { return vector_dot(sz, data, a.data); }
   inline int size() const { return sz; }
   inline int Size() const { return sz; }
   inline int bytes() const { return size()*sizeof(T); }
   void allocate(const int X, const int Y =1,
                 const int Z =1, const int D =1,
                 const bool transposed = false)
   {
      d[0]=X; d[1]=Y; d[2]=Z; d[3]=D;
      sz=d[0]*d[1]*d[2]*d[3];
      data=(T*) rmalloc<T>::operator new (sz);
   }
   inline T& operator[](const int x) { return data[x]; }
   inline T& operator()(const int x, const int y)
   {
      return data[x + d[0]*y];
   }
   inline T& operator()(const int x, const int y, const int z)
   {
      return data[x + d[0]*(y + d[1]*z)];
   }
   void Print(std::ostream& out= std::cout, int width = 8) const
   {
      T *h_data = (double*) ::malloc(bytes());
      rmemcpy::rDtoH(h_data,data,bytes());
      for (int i=0; i<sz; i+=1)
         if (sizeof(T)==8) { printf("\n\t[%ld] %.15e",i,h_data[i]); }
         else { printf("\n\t[%ld] %d",i,h_data[i]); }
      free(h_data);
   }
};

// Partial Specializations for xyz==FALSE ************************************
template <class T> class CudaArray<T,false> : public rmalloc<T>
{
private:
   static const int DIM = 4;
   T* data = NULL;
   int sz,d[DIM];
public:
   CudaArray():data(NULL),sz(0),d{0,0,0,0} {}
   CudaArray(const int d0) {allocate(d0);}
   CudaArray(const CudaArray<T,false> &r) {assert(false);}
   ~CudaArray() {rmalloc<T>::operator delete (data);}
   CudaArray& operator=(Array<T> &a)
   {
      rmemcpy::rHtoD(data,a.GetData(),a.Size()*sizeof(T));
      return *this;
   }
   inline int* dim() { return &d[0]; }
   inline T* ptr() { return data; }
   inline T* GetData() const { return data; }
   inline const T* ptr() const { return data; }
   inline operator T* () { return data; }
   inline operator const T* () const { return data; }
   double operator* (const CudaArray& a) const { return vector_dot(sz, data, a.data); }
   inline int size() const { return sz; }
   inline int Size() const { return sz; }
   inline int bytes() const { return size()*sizeof(T); }
   void allocate(const int X, const int Y =1,
                 const int Z =1, const int D =1,
                 const bool transposed = false)
   {
      d[0]=X; d[1]=Y; d[2]=Z; d[3]=D;
      sz=d[0]*d[1]*d[2]*d[3];
      assert(sz>0);
      data=(T*) rmalloc<T>::operator new (sz);
#define xsw(a,b) a^=b^=a^=b
      if (transposed) { xsw(d[0],d[1]); }
      for (int i=1,b=d[0]; i<DIM; xsw(d[i],b),++i)
      {
         d[i]*=d[i-1];
      }
      d[0]=1;
      if (transposed) { xsw(d[0],d[1]); }
   }
   inline T& operator[](const int x) { return data[x]; }
   inline T& operator()(const int x, const int y)
   {
      return data[d[0]*x + d[1]*y];
   }
   inline T& operator()(const int x, const int y, const int z)
   {
      return data[d[0]*x + d[1]*y + d[2]*z];
   }
   void Print(std::ostream& out= std::cout, int width = 8) const
   {
      T *h_data = (double*) ::malloc(bytes());
      rmemcpy::rDtoH(h_data,data,bytes());
      for (int i=0; i<sz; i+=1)
         if (sizeof(T)==8) { printf("\n\t[%ld] %.15e",i,h_data[i]); }
         else { printf("\n\t[%ld] %d",i,h_data[i]); }
      free(h_data);
   }
};

} // mfem

#endif // LAGHOS_CUDA_ARRAY

