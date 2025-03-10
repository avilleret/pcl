/*
 * Software License Agreement (BSD License)
 *
 *  Point Cloud Library (PCL) - www.pointclouds.org
 *  Copyright (c) 2010, Willow Garage, Inc.
 *  Copyright (c) 2012-, Open Perception, Inc.
 *
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of the copyright holder(s) nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */
#ifndef PCL_POLYNOMIAL_CALCULATIONS_HPP_
#define PCL_POLYNOMIAL_CALCULATIONS_HPP_

////////////////////////////////////

template <typename real>
pcl::PolynomialCalculationsT<real>::PolynomialCalculationsT ()
{
}

////////////////////////////////////

template <typename real>
pcl::PolynomialCalculationsT<real>:: ~PolynomialCalculationsT ()
{
}

////////////////////////////////////

template <typename real>
inline void
  pcl::PolynomialCalculationsT<real>::Parameters::setZeroValue (real new_zero_value)
{
  zero_value = new_zero_value;
  sqr_zero_value = zero_value*zero_value;
}

////////////////////////////////////

template <typename real>
inline void
  pcl::PolynomialCalculationsT<real>::solveLinearEquation (real a, real b, std::vector<real>& roots) const
{
  //std::cout << "Trying to solve "<<a<<"x + "<<b<<" = 0\n";

  if (isNearlyZero (b))
  {
    roots.push_back (0.0);
  }
  if (!isNearlyZero (a/b))
  {
    roots.push_back (-b/a);
  }

#if 0
  std::cout << __PRETTY_FUNCTION__ << ": Found "<<roots.size ()<<" roots.\n";
  for (unsigned int i=0; i<roots.size (); i++)
  {
    real x=roots[i];
    real result = a*x + b;
    if (!isNearlyZero (result))
    {
      std::cout << "Something went wrong during solving of polynomial "<<a<<"x + "<<b<<" = 0\n";
      //roots.clear ();
    }
    std::cout << "Root "<<i<<" = "<<roots[i]<<". ("<<a<<"x^ + "<<b<<" = "<<result<<")\n";
  }
#endif
}

////////////////////////////////////

template <typename real>
inline void
  pcl::PolynomialCalculationsT<real>::solveQuadraticEquation (real a, real b, real c, std::vector<real>& roots) const
{
  //std::cout << "Trying to solve "<<a<<"x^2 + "<<b<<"x + "<<c<<" = 0\n";

  if (isNearlyZero (a))
  {
    //std::cout << "Highest order element is 0 => Calling solveLineaqrEquation.\n";
    solveLinearEquation (b, c, roots);
    return;
  }

  if (isNearlyZero (c))
  {
    roots.push_back (0.0);
    //std::cout << "Constant element is 0 => Adding root 0 and calling solveLinearEquation.\n";
    std::vector<real> tmpRoots;
    solveLinearEquation (a, b, tmpRoots);
    for (const auto& tmpRoot: tmpRoots)
      if (!isNearlyZero (tmpRoot))
        roots.push_back (tmpRoot);
    return;
  }

  real tmp = b*b - 4*a*c;
  if (tmp>0)
  {
    tmp = sqrt (tmp);
    real tmp2 = 1.0/ (2*a);
    roots.push_back ( (-b + tmp)*tmp2);
    roots.push_back ( (-b - tmp)*tmp2);
  }
  else if (sqrtIsNearlyZero (tmp))
  {
    roots.push_back (-b/ (2*a));
  }

#if 0
  std::cout << __PRETTY_FUNCTION__ << ": Found "<<roots.size ()<<" roots.\n";
  for (unsigned int i=0; i<roots.size (); i++)
  {
    real x=roots[i], x2=x*x;
    real result = a*x2 + b*x + c;
    if (!isNearlyZero (result))
    {
      std::cout << "Something went wrong during solving of polynomial "<<a<<"x^2 + "<<b<<"x + "<<c<<" = 0\n";
      //roots.clear ();
    }
    //std::cout << "Root "<<i<<" = "<<roots[i]<<". ("<<a<<"x^2 + "<<b<<"x + "<<c<<" = "<<result<<")\n";
  }
#endif
}

////////////////////////////////////

template<typename real>
inline void
  pcl::PolynomialCalculationsT<real>::solveCubicEquation (real a, real b, real c, real d, std::vector<real>& roots) const
{
  //std::cout << "Trying to solve "<<a<<"x^3 + "<<b<<"x^2 + "<<c<<"x + "<<d<<" = 0\n";

  if (isNearlyZero (a))
  {
    //std::cout << "Highest order element is 0 => Calling solveQuadraticEquation.\n";
    solveQuadraticEquation (b, c, d, roots);
    return;
  }

  if (isNearlyZero (d))
  {
    roots.push_back (0.0);
    //std::cout << "Constant element is 0 => Adding root 0 and calling solveQuadraticEquation.\n";
    std::vector<real> tmpRoots;
    solveQuadraticEquation (a, b, c, tmpRoots);
    for (const auto& tmpRoot: tmpRoots)
      if (!isNearlyZero (tmpRoot))
        roots.push_back (tmpRoot);
    return;
  }

  double a2 = a*a,
         a3 = a2*a,
         b2 = b*b,
         b3 = b2*b,
         alpha = ( (3.0*a*c-b2)/ (3.0*a2)),
         beta  = (2*b3/ (27.0*a3)) - ( (b*c)/ (3.0*a2)) + (d/a),
         alpha2 = alpha*alpha,
         alpha3 = alpha2*alpha,
         beta2 = beta*beta;

  // Value for resubstitution:
  double resubValue = b/ (3*a);

  //std::cout << "Trying to solve y^3 + "<<alpha<<"y + "<<beta<<"\n";

  double discriminant = (alpha3/27.0) + 0.25*beta2;

  //std::cout << "Discriminant is "<<discriminant<<"\n";

  if (isNearlyZero (discriminant))
  {
    if (!isNearlyZero (alpha) || !isNearlyZero (beta))
    {
      roots.push_back ( (-3.0*beta)/ (2.0*alpha) - resubValue);
      roots.push_back ( (3.0*beta)/alpha - resubValue);
    }
    else
    {
      roots.push_back (-resubValue);
    }
  }
  else if (discriminant > 0)
  {
    double sqrtDiscriminant = sqrt (discriminant);
    double d1 = -0.5*beta + sqrtDiscriminant,
           d2 = -0.5*beta - sqrtDiscriminant;
    if (d1 < 0)
      d1 = -pow (-d1, 1.0/3.0);
    else
      d1 = pow (d1, 1.0/3.0);

    if (d2 < 0)
      d2 = -pow (-d2, 1.0/3.0);
    else
      d2 = pow (d2, 1.0/3.0);

    //std::cout << PVAR (d1)<<", "<<PVAR (d2)<<"\n";
    roots.push_back (d1 + d2 - resubValue);
  }
  else
  {
    double tmp1 = sqrt (- (4.0/3.0)*alpha),
           tmp2 = std::acos (-sqrt (-27.0/alpha3)*0.5*beta)/3.0;
    roots.push_back (tmp1*std::cos (tmp2) - resubValue);
    roots.push_back (-tmp1*std::cos (tmp2 + M_PI/3.0) - resubValue);
    roots.push_back (-tmp1*std::cos (tmp2 - M_PI/3.0) - resubValue);
  }

#if 0
  std::cout << __PRETTY_FUNCTION__ << ": Found "<<roots.size ()<<" roots.\n";
  for (unsigned int i=0; i<roots.size (); i++)
  {
    real x=roots[i], x2=x*x, x3=x2*x;
    real result = a*x3 + b*x2 + c*x + d;
    if (std::abs (result) > 1e-4)
    {
      std::cout << "Something went wrong:\n";
      //roots.clear ();
    }
    std::cout << "Root "<<i<<" = "<<roots[i]<<". ("<<a<<"x^3 + "<<b<<"x^2 + "<<c<<"x + "<<d<<" = "<<result<<")\n";
  }
  std::cout << "\n\n";
#endif
}

////////////////////////////////////

template<typename real>
inline void
  pcl::PolynomialCalculationsT<real>::solveQuarticEquation (real a, real b, real c, real d, real e,
                                                            std::vector<real>& roots) const
{
  //std::cout << "Trying to solve "<<a<<"x^4 + "<<b<<"x^3 + "<<c<<"x^2 + "<<d<<"x + "<<e<<" = 0\n";

  if (isNearlyZero (a))
  {
    //std::cout << "Highest order element is 0 => Calling solveCubicEquation.\n";
    solveCubicEquation (b, c, d, e, roots);
    return;
  }

  if (isNearlyZero (e))
  {
    roots.push_back (0.0);
    //std::cout << "Constant element is 0 => Adding root 0 and calling solveCubicEquation.\n";
    std::vector<real> tmpRoots;
    solveCubicEquation (a, b, c, d, tmpRoots);
    for (const auto& tmpRoot: tmpRoots)
      if (!isNearlyZero (tmpRoot))
        roots.push_back (tmpRoot);
    return;
  }

  double root1, root2, root3, root4,
         a2 = a*a,
         a3 = a2*a,
         a4 = a2*a2,
         b2 = b*b,
         b3 = b2*b,
         b4 = b2*b2,
         alpha = ( (-3.0*b2)/ (8.0*a2)) + (c/a),
         beta  = (b3/ (8.0*a3)) - ( (b*c)/ (2.0*a2)) + (d/a),
         gamma = ( (-3.0*b4)/ (256.0*a4)) + ( (c*b2)/ (16.0*a3)) - ( (b*d)/ (4.0*a2)) + (e/a),
         alpha2 = alpha*alpha;

  // Value for resubstitution:
  double resubValue = b/ (4*a);

  //std::cout << "Trying to solve y^4 + "<<alpha<<"y^2 + "<<beta<<"y + "<<gamma<<"\n";

  if (isNearlyZero (beta))
  {  // y^4 + alpha*y^2 + gamma\n";
    //std::cout << "Using beta=0 condition\n";
    std::vector<real> tmpRoots;
    solveQuadraticEquation (1.0, alpha, gamma, tmpRoots);
    for (const auto& quadraticRoot: tmpRoots)
    {
      if (sqrtIsNearlyZero (quadraticRoot))
      {
        roots.push_back (-resubValue);
      }
      else if (quadraticRoot > 0.0)
      {
        root1 = sqrt (quadraticRoot);
        roots.push_back (root1 - resubValue);
        roots.push_back (-root1 - resubValue);
      }
    }
  }
  else
  {
    //std::cout << "beta != 0\n";
    double alpha3 = alpha2*alpha,
           beta2 = beta*beta,
           p = (-alpha2/12.0)-gamma,
           q = (-alpha3/108.0)+ ( (alpha*gamma)/3.0)- (beta2/8.0),
           q2 = q*q,
           p3 = p*p*p,
           u = (0.5*q) + sqrt ( (0.25*q2)+ (p3/27.0));
    if (u > 0.0)
      u = pow (u, 1.0/3.0);
    else if (isNearlyZero (u))
      u = 0.0;
    else
      u = -pow (-u, 1.0/3.0);

    double y = (-5.0/6.0)*alpha - u;
    if (!isNearlyZero (u))
      y += p/ (3.0*u);

    double w = alpha + 2.0*y;

    if (w > 0)
    {
      w = sqrt (w);
    }
    else if (isNearlyZero (w))
    {
      w = 0;
    }
    else
    {
      //std::cout << "Found no roots\n";
      return;
    }

    double tmp1 = - (3.0*alpha + 2.0*y + 2.0* (beta/w)),
           tmp2 = - (3.0*alpha + 2.0*y - 2.0* (beta/w));

    if (tmp1 > 0)
    {
      tmp1 = sqrt (tmp1);
      root1 = - (b/ (4.0*a)) + 0.5* (w+tmp1);
      root2 = - (b/ (4.0*a)) + 0.5* (w-tmp1);
      roots.push_back (root1);
      roots.push_back (root2);
    }
    else if (isNearlyZero (tmp1))
    {
      root1 = - (b/ (4.0*a)) + 0.5*w;
      roots.push_back (root1);
    }

   if (tmp2 > 0)
   {
      tmp2 = sqrt (tmp2);
      root3 = - (b/ (4.0*a)) + 0.5* (-w+tmp2);
      root4 = - (b/ (4.0*a)) + 0.5* (-w-tmp2);
      roots.push_back (root3);
      roots.push_back (root4);
    }
    else if (isNearlyZero (tmp2))
    {
      root3 = - (b/ (4.0*a)) - 0.5*w;
      roots.push_back (root3);
    }

    //std::cout << "Test: " << alpha<<", "<<beta<<", "<<gamma<<", "<<p<<", "<<q<<", "<<u <<", "<<y<<", "<<w<<"\n";
  }

#if 0
  std::cout << __PRETTY_FUNCTION__ << ": Found "<<roots.size ()<<" roots.\n";
  for (unsigned int i=0; i<roots.size (); i++)
  {
    real x=roots[i], x2=x*x, x3=x2*x, x4=x2*x2;
    real result = a*x4 + b*x3 + c*x2 + d*x + e;
    if (std::abs (result) > 1e-4)
    {
      std::cout << "Something went wrong:\n";
      //roots.clear ();
    }
    std::cout << "Root "<<i<<" = "<<roots[i]
         << ". ("<<a<<"x^4 + "<<b<<"x^3 + "<<c<<"x^2 + "<<d<<"x + "<<e<<" = "<<result<<")\n";
  }
  std::cout << "\n\n";
#endif
}

////////////////////////////////////

template<typename real>
inline pcl::BivariatePolynomialT<real>
  pcl::PolynomialCalculationsT<real>::bivariatePolynomialApproximation (
      std::vector<Eigen::Matrix<real, 3, 1>, Eigen::aligned_allocator<Eigen::Matrix<real, 3, 1> > >& samplePoints, unsigned int polynomial_degree, bool& error) const
{
  pcl::BivariatePolynomialT<real> ret;
  error = bivariatePolynomialApproximation (samplePoints, polynomial_degree, ret);
  return ret;
}

////////////////////////////////////

template<typename real>
inline bool
  pcl::PolynomialCalculationsT<real>::bivariatePolynomialApproximation (
      std::vector<Eigen::Matrix<real, 3, 1>, Eigen::aligned_allocator<Eigen::Matrix<real, 3, 1> > >& samplePoints, unsigned int polynomial_degree,
      pcl::BivariatePolynomialT<real>& ret) const
{
  //MEASURE_FUNCTION_TIME;
  unsigned int parameters_size = BivariatePolynomialT<real>::getNoOfParametersFromDegree (polynomial_degree);
  //std::cout << PVARN (parameters_size);

  //std::cout << "Searching for the "<<parameters_size<<" parameters for the bivariate polynom of degree "
  //     << polynomial_degree<<" using "<<samplePoints.size ()<<" points.\n";

  if (parameters_size > samplePoints.size ()) // Too many parameters for this number of equations (points)?
  {
    return false;
    // Reduce degree of polynomial
    //polynomial_degree = (unsigned int) (0.5f* (std::sqrt (8*samplePoints.size ()+1) - 3));
    //parameters_size = BivariatePolynomialT<real>::getNoOfParametersFromDegree (polynomial_degree);
    //std::cout << "Not enough points, so degree of polynomial was decreased to "<<polynomial_degree
    //     << " ("<<samplePoints.size ()<<" points => "<<parameters_size<<" parameters)\n";
  }

  ret.setDegree (polynomial_degree);

  //double coeffStuffStartTime=-get_time ();
  Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> A (parameters_size, parameters_size);
  A.setZero();
  Eigen::Matrix<real, Eigen::Dynamic, 1> b (parameters_size);
  b.setZero();
  real currentX, currentY, currentZ;
  real tmpX, tmpY;
  real *tmpC = new real[parameters_size];
  real* tmpCEndPtr = &tmpC[parameters_size-1];
  for (const auto& point: samplePoints)
  {
    currentX= point[0]; currentY= point[1]; currentZ= point[2];
    //std::cout << "current point: "<<currentX<<","<<currentY<<" => "<<currentZ<<"\n";
    //unsigned int posInC = parameters_size-1;
    real* tmpCPtr = tmpCEndPtr;
    tmpX = 1.0;
    for (unsigned int xDegree=0; xDegree<=polynomial_degree; ++xDegree)
    {
      tmpY = 1.0;
      for (unsigned int yDegree=0; yDegree<=polynomial_degree-xDegree; ++yDegree)
      {
        * (tmpCPtr--) = tmpX*tmpY;
        //std::cout << "x="<<currentX<<", y="<<currentY<<", Pos "<<posInC--<<": "<<tmpX<<"*"<<tmpY<<"="<<tmpC[posInC]<<"\n";
        tmpY *= currentY;
      }
      tmpX *= currentX;
    }

    real* APtr = &A(0,0);
    real* bPtr = &b[0];
    real* tmpCPtr1=tmpC;
    for (unsigned int i=0; i<parameters_size; ++i)
    {
      * (bPtr++) += currentZ * *tmpCPtr1;

      real* tmpCPtr2=tmpC;
      for (unsigned int j=0; j<parameters_size; ++j)
      {
        * (APtr++) += *tmpCPtr1 * * (tmpCPtr2++);
      }

      ++tmpCPtr1;
    }
    //A += DMatrix<real>::outProd (tmpC);
    //b += currentZ * tmpC;
  }
  //std::cout << "Calculating matrix A and vector b (size "<<b.size ()<<") from "<<samplePoints.size ()<<" points took "
       //<< (coeffStuffStartTime+get_time ())*1000<<"ms using constant memory.\n";
    //std::cout << PVARC (A)<<PVARN (b);


  //double coeffStuffStartTime=-get_time ();
  //DMatrix<real> A (parameters_size, parameters_size);
  //DVector<real> b (parameters_size);
  //real currentX, currentY, currentZ;
  //unsigned int posInC;
  //real tmpX, tmpY;
  //DVector<real> tmpC (parameters_size);
  //for (typename std::vector<Eigen::Matrix<real, 3, 1>, Eigen::aligned_allocator<Eigen::Matrix<real, 3, 1> > >::const_iterator it=samplePoints.begin ();
  //     it!=samplePoints.end (); ++it)
  //{
    //currentX= (*it)[0]; currentY= (*it)[1]; currentZ= (*it)[2];
    ////std::cout << "x="<<currentX<<", y="<<currentY<<"\n";
    //posInC = parameters_size-1;
    //tmpX = 1.0;
    //for (unsigned int xDegree=0; xDegree<=polynomial_degree; xDegree++)
    //{
      //tmpY = 1.0;
      //for (unsigned int yDegree=0; yDegree<=polynomial_degree-xDegree; yDegree++)
      //{
        //tmpC[posInC] = tmpX*tmpY;
        ////std::cout << "x="<<currentX<<", y="<<currentY<<", Pos "<<posInC<<": "<<tmpX<<"*"<<tmpY<<"="<<tmpC[posInC]<<"\n";
        //tmpY *= currentY;
        //posInC--;
      //}
      //tmpX *= currentX;
    //}
    //A += DMatrix<real>::outProd (tmpC);
    //b += currentZ * tmpC;
  //}
  //std::cout << "Calculating matrix A and vector b (size "<<b.size ()<<") from "<<samplePoints.size ()<<" points took "
       //<< (coeffStuffStartTime+get_time ())*1000<<"ms.\n";

  Eigen::Matrix<real, Eigen::Dynamic, 1> parameters;
  //double choleskyStartTime=-get_time ();
  //parameters = A.choleskySolve (b);
  //std::cout << "Cholesky took "<< (choleskyStartTime+get_time ())*1000<<"ms.\n";

  //double invStartTime=-get_time ();
  parameters = A.inverse () * b;
  //std::cout << "Inverse took "<< (invStartTime+get_time ())*1000<<"ms.\n";

  //std::cout << PVARC (A)<<PVARC (b)<<PVARN (parameters);

  real inversionCheckResult = (A*parameters - b).norm ();
  if (inversionCheckResult > 1e-5)
  {
    //std::cout << "Inversion result: "<< inversionCheckResult<<" for matrix "<<A<<"\n";
    return false;
  }

  std::copy_n(parameters.data(), parameters_size, ret.parameters);

  //std::cout << "Resulting polynomial is "<<ret<<"\n";

  //Test of gradient: ret.calculateGradient ();

  delete [] tmpC;
  return true;
}

#endif      // PCL_POLYNOMIAL_CALCULATIONS_HPP_

