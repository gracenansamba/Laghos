// Copyright (c) 2010, Lawrence Livermore National Security, LLC. Produced at
// the Lawrence Livermore National Laboratory. LLNL-CODE-443211. All Rights
// reserved. See file COPYRIGHT for details.
//
// This file is part of the MFEM library. For more information and source code
// availability see http://mfem.org.
//
// MFEM is free software; you can redistribute it and/or modify it under the
// terms of the GNU Lesser General Public License (as published by the Free
// Software Foundation) version 2.1 dated February 1999.

#ifndef MFEM_LAGHOS_QUPDATE_DOF2QUAD
#define MFEM_LAGHOS_QUPDATE_DOF2QUAD

namespace mfem
{

namespace hydrodynamics {

   // *****************************************************************************
   // * Dof2QuadScalar
   // *****************************************************************************
   void Dof2QuadScalar(ParFiniteElementSpace&, const IntegrationRule&,
                       const double*, double*);

   // *****************************************************************************
   // * Dof2QuadGrad
   // *****************************************************************************
   void Dof2QuadGrad(ParFiniteElementSpace&, const IntegrationRule&,
                     /*const*/ double*, double**);
   
} // namespace hydrodynamics
   
} // namespace mfem

#endif // MFEM_LAGHOS_QUPDATE_DOF2QUAD
