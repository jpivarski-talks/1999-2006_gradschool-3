#if !defined(HISTOGRAMINTERFACE_HIOPERATORS_H)
#define HISTOGRAMINTERFACE_HIOPERATORS_H

struct AddOp
{
   static inline double execute( const float scale, 
				 const float& left, const float& right)
   {
      return scale*(left + right);
   }

   static inline double 
   executeError( const float& sigma1, const float& sigma2,
		 const float& scalar1, const float& scalar2,
		 const float& value1, const float& value2 )
   {
      return (scalar1*scalar1 * sigma1*sigma1)
	 + (scalar2*scalar2 * sigma2*sigma2);
   }
};

struct SubOp
{
   static inline double execute( const float scale,
				 const float& left, const float& right)
   {
      return scale * (left - right);
   }

   static inline double 
   executeError( const float& sigma1, const float& sigma2,
		 const float& scalar1, const float& scalar2,
		 const float& value1, const float& value2 )
   {
      return (scalar1*scalar1 * sigma1*sigma1)
	 + (scalar2*scalar2 * sigma2*sigma2);
   }
};

struct MulOp
{
   static inline double execute( const float scale,
				 const float& left, const float& right)
   {
      return scale * left * right;
   }

   static inline double 
   executeError( const float& sigma1, const float& sigma2,
		 const float& scalar1, const float& scalar2,
		 const float& value1, const float& value2 )
   {
      return (scalar1*scalar1 * scalar2*scalar2)*
	  ( ( value1 * value1 * sigma2 * sigma2 )
	 + ( value2 * value2 * sigma1 * sigma1 ) );
   } 

};
   
struct DivOp
{
   static inline double execute( const float scale,
				 const float& left, const float& right)
   {
      float value = 0.;

      if( right != 0. )
      {
	 value = scale*left/right;	 
      }
      return value;
   }

   static inline double 
   executeError( const float& sigma1, const float& sigma2,
		 const float& scalar1, const float& scalar2,
		 const float& value1, const float& value2 )
   {
      if ( scalar2 == 0. || value2 == 0. )
      {
	 return 0.;
      }

      return ((scalar1*scalar1)/(scalar2*scalar2))*
	  ( ( sigma1 * sigma1/(value2*value2) )
	 + ( value1 * value1 * sigma2 * sigma2 )/
	    (value2*value2*value2*value2) );
   } 
};
   
#endif /* HISTOGRAMINTERFACE_HIOPERATORS_H */
