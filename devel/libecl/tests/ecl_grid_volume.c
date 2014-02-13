/*
   Copyright (C) 2013  Statoil ASA, Norway. 
    
   The file 'ecl_grid_volume.c' is part of ERT - Ensemble based Reservoir Tool. 
    
   ERT is free software: you can redistribute it and/or modify 
   it under the terms of the GNU General Public License as published by 
   the Free Software Foundation, either version 3 of the License, or 
   (at your option) any later version. 
    
   ERT is distributed in the hope that it will be useful, but WITHOUT ANY 
   WARRANTY; without even the implied warranty of MERCHANTABILITY or 
   FITNESS FOR A PARTICULAR PURPOSE.   
    
   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html> 
   for more details. 
*/
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include <ert/util/test_util.h>
#include <ert/util/util.h>

#include <ert/ecl/ecl_grid.h>
#include <ert/ecl/ecl_file.h>
#include <ert/ecl/ecl_kw.h>





int main(int argc , char ** argv) {
  const char * path_case = argv[1];
  char * grid_file = ecl_util_alloc_filename( NULL , path_case , ECL_EGRID_FILE , false , 0 );
  char * init_file = ecl_util_alloc_filename( NULL , path_case , ECL_INIT_FILE , false , 0 );
  
  ecl_file_type * init = ecl_file_open( init_file , 0 );
  ecl_grid_type * grid = ecl_grid_alloc( grid_file );
  const ecl_kw_type * poro_kw = ecl_file_iget_named_kw( init , "PORO" , 0 );
  const ecl_kw_type * porv_kw = ecl_file_iget_named_kw( init , "PORV" , 0 );
  bool error_found = false;
  
  int error_count = 0;
  int iactive;
  for (iactive = 0; iactive < ecl_grid_get_nactive( grid ); ++iactive) {
    int iglobal = ecl_grid_get_global_index1A( grid , iactive );
    
    double grid_volume = ecl_grid_get_cell_volume1( grid , iglobal );
    double eclipse_volume = ecl_kw_iget_float( porv_kw , iglobal ) / ecl_kw_iget_float( poro_kw , iactive );
    
    if (!util_double_approx_equal__( grid_volume , eclipse_volume , 1e-3)) {
      printf("Error in cell: %d V: %g    V2: %g \n",iglobal , grid_volume , eclipse_volume );
      error_count++;
      error_found = true;
    }
  }
  



  ecl_grid_free( grid );
  ecl_file_close( init );
  free( grid_file );
  free( init_file );
  if (error_found) {
    printf("Error_count: %d \n",error_count);
    exit(1);
  }  else
    exit(0);
}