/********************************************************************************/
/*     888888    888888888   88     888  88888   888      888    88888888       */
/*   8       8   8           8 8     8     8      8        8    8               */
/*  8            8           8  8    8     8      8        8    8               */
/*  8            888888888   8   8   8     8      8        8     8888888        */
/*  8      8888  8           8    8  8     8      8        8            8       */
/*   8       8   8           8     8 8     8      8        8            8       */
/*     888888    888888888  888     88   88888     88888888     88888888        */
/*                                                                              */
/*       A Three-Dimensional General Purpose Semiconductor Simulator.           */
/*                                                                              */
/*                                                                              */
/*  Copyright (C) 2007-2008                                                     */
/*  Cogenda Pte Ltd                                                             */
/*                                                                              */
/*  Please contact Cogenda Pte Ltd for license information                      */
/*                                                                              */
/*  Author: Gong Ding   gdiso@ustc.edu                                          */
/*                                                                              */
/********************************************************************************/



#ifndef __fvm_node_data_pml_h__
#define __fvm_node_data_pml_h__

#include "petsc.h"
#include "fvm_node_data.h"




/**
 *  FVM nodal data for PML region
 */
class FVM_PML_NodeData : public FVM_NodeData
{

  public:

    /**
     * the independent variable for vacuum region
     */
    enum   PMLData
    {
      /**
       * electrostatic potential
       */
      _psi_,

      /**
       * the density of the material
       */
      _density_,

      /**
       * electron affinity
       */
      _affinity_,

      /**
       * the dielectric permittivity
       */
      _eps_,

      /**
       * the megnetic permeability
       */
      _mu_,

      /**
       * electrostatic potential at previous time step
       */
      _psi_last_,

      /**
       * last enum number
       */
      ScalarDataCount
    };


    /**
     * the vector auxiliary variable for PML region
     */
    enum PMLAuxVecData
    {
      /**
       * electrical field of incident optical wave
       */
      //_OpE_,

      /**
       * magnetic field of incident optical wave
       */
      //_OpH_,

      /**
       * electrical field
       */
      _E_,

      /**
       * last enum number
       */
      VectorDataCount
    };


    /**
     * the complex auxiliary variable for PML region
     */
    enum PMLAuxComplexData
    {
      /**
       * electrical field of incident optical wave
       */
      _OpE_complex_=0,

      /**
       * magnetic field of incident optical wave
       */
      _OpH_complex_,

      /**
       * last enum number
       */
      ComplexDataCount
    };

  public:
    /**
     * constructor
     */
    FVM_PML_NodeData ( DataStorage * data_storage , const std::map<std::string, SimulationVariable> & variables )
    :FVM_NodeData ( data_storage , variables )
    {}

    /**
     * destructor
     */
    virtual ~FVM_PML_NodeData()  {}

  public:
    /**
     * @return the solution variable number
     */
    static size_t n_scalar()
    { return static_cast<unsigned int> ( ScalarDataCount ) ; /* return last enum */ }

    /**
     * @return the complex variable number
     */
    static size_t n_complex()
    { return static_cast<unsigned int> ( ComplexDataCount ) ; /* return last enum */ }

    /**
     * @return the vector variable number
     */
    static size_t n_vector()
    { return static_cast<unsigned int> ( VectorDataCount ) ; /* return last enum */ }

    /**
     * @return the tensor variable number
     */
    static size_t n_tensor()
    { return 0; }

    /**
     * @return the data type
     */
    virtual NodeDataType type() const
    { return FVM_NodeData::VacuumData; }


  public:

    /**
     * @return data by enum name
     */
    virtual PetscScalar  get_variable_real ( SolutionVariable variable ) const
    {
      switch ( variable )
      {
        case POTENTIAL   :  return  psi();                            /* potential */
        case E_FIELD     :  return  E().size();                       /* electric field */
        case QFN         :  return  psi();                            /* electron quasi-Fermi level */
        case QFP         :  return  psi();                            /* hole quasi-Fermi level */
        default          :  return  0.0;
      }
    }

    /**
     * set variable by enum name
     */
    virtual void set_variable_real ( SolutionVariable variable, PetscScalar value )
    {
      switch ( variable )
      {
        case POTENTIAL   :  psi() = value;                             /* potential */
        default          :  return;
      }
    }

    /**
     * @return true when this variable valid
     */
    virtual bool is_variable_valid ( SolutionVariable variable )  const
    {
      switch ( variable )
      {
        case POTENTIAL   :  return true;
        default          :  return false;
      }
    }
    //--------------------------------------------------------------------
    // data access function
    //--------------------------------------------------------------------

    /**
     * @return the statistic potential
     */
    virtual PetscScalar         psi()        const
    { return _data_storage->scalar ( _psi_, _offset ); }

    /**
     * @return the statistic potential
     */
    virtual PetscScalar &       psi()
    { return _data_storage->scalar ( _psi_, _offset ); }


    /**
     * @return the statistic potential
     */
    virtual std::complex<PetscScalar>         psi_ac()          const
    { return _data_storage->complex ( _psi_, _offset ); }

    /**
     * @return the writable reference to statistic potential
     */
    virtual std::complex<PetscScalar> &       psi_ac()
    { return _data_storage->complex ( _psi_, _offset ); }

    /**
     * @return the statistic potential at previous time step
     */
    virtual PetscScalar         psi_last()          const
    { return _data_storage->scalar ( _psi_last_, _offset ); }

    /**
     * @return the writable reference to statistic potential at previous time step
     */
    virtual PetscScalar &       psi_last()
    { return _data_storage->scalar ( _psi_last_, _offset ); }


    /**
     * @return the complex E file. only used by EM FEM solver
     */
    virtual std::complex<PetscScalar>         OptE_complex()          const
    { return _data_storage->complex ( _OpE_complex_, _offset ); }

    /**
     * @return the writable reference to complex E file. only used by EM FEM solver
     */
    virtual std::complex<PetscScalar> &       OptE_complex()
    { return _data_storage->complex ( _OpE_complex_, _offset ); }

    /**
     * @return the complex H file. only used by EM FEM solver
     */
    virtual std::complex<PetscScalar>         OptH_complex()          const
    { return _data_storage->complex ( _OpH_complex_, _offset ); }

    /**
     * @return the writable reference to complex H file. only used by EM FEM solver
     */
    virtual std::complex<PetscScalar> &       OptH_complex()
    { return _data_storage->complex ( _OpH_complex_, _offset ); }


    /**
     * @return the electron affinity
     */
    virtual PetscScalar         affinity()          const
    { return _data_storage->scalar ( _affinity_, _offset ); }

    /**
     * @return the writable reference to the electron affinity
     */
    virtual PetscScalar &       affinity()
    { return _data_storage->scalar ( _affinity_, _offset ); }



    /**
     * @return the mass density of the material
     */
    virtual PetscScalar         density()          const
    { return _data_storage->scalar ( _density_, _offset ); }

    /**
     * @return the writable reference to the mass density of the material
     */
    virtual PetscScalar &       density()
    { return _data_storage->scalar ( _density_, _offset ); }



    /**
     * @return the dielectric permittivity
     */
    virtual PetscScalar         eps()          const
    { return _data_storage->scalar ( _eps_, _offset ); }

    /**
     * @return the writable reference to the dielectric permittivity
     */
    virtual PetscScalar &       eps()
    { return _data_storage->scalar ( _eps_, _offset ); }



    /**
     * @return the megnetic permeability
     */
    virtual PetscScalar         mu()          const
    { return _data_storage->scalar ( _mu_, _offset ); }

    /**
     * @return the writable reference to the megnetic permeability
     */
    virtual PetscScalar &       mu()
    { return _data_storage->scalar ( _mu_, _offset ); }


    /**
     * @return the electrical field
     */
    virtual VectorValue<PetscScalar> E()       const
    { return _data_storage->vector ( _E_, _offset );}


    /**
     * @return the writable reference to electrical field
     */
    virtual VectorValue<PetscScalar> & E()
    { return _data_storage->vector ( _E_, _offset );}


};




#endif
