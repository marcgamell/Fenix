/*
//@HEADER
// ************************************************************************
//
//
//            _|_|_|_|  _|_|_|_|  _|      _|  _|_|_|  _|      _|
//            _|        _|        _|_|    _|    _|      _|  _|
//            _|_|_|    _|_|_|    _|  _|  _|    _|        _|
//            _|        _|        _|    _|_|    _|      _|  _|
//            _|        _|_|_|_|  _|      _|  _|_|_|  _|      _|
//
//
//
//
// Copyright (C) 2016 Rutgers University and Sandia Corporation
//
// Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
// the U.S. Government retains certain rights in this software.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
// 1. Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the Corporation nor the names of the
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY SANDIA CORPORATION "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL SANDIA CORPORATION OR THE
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author Marc Gamell, Eric Valenzuela, Keita Teranishi, Manish Parashar,
//        Rob Van der Wijngaart, and Michael Heroux
//
// Questions? Contact Keita Teranishi (knteran@sandia.gov) and
//                    Marc Gamell (mgamell@cac.rutgers.edu)
//
// ************************************************************************
//@HEADER
*/
#include "fenix_process_recovery.h"
#include <mpi.h>
#include "fenix_ext.h"

#ifdef OPEN_MPI
#define MPI_SEND_BUFF_TYPE void *
#else
#define MPI_SEND_BUFF_TYPE const void *
#endif

int MPI_Comm_size(MPI_Comm comm, int *size) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Comm_size(comm, size);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Comm_size(*__fenix_g_new_world, size);
    } else {
      ret = PMPI_Comm_size(comm, size);
    }
  }
  __fenix_test_MPI(ret, "MPI_Comm_size");
  return ret;
}

int MPI_Comm_dup(MPI_Comm comm, MPI_Comm *newcomm) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Comm_dup(comm, newcomm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Comm_dup(*__fenix_g_new_world, newcomm);
    } else {
      ret = PMPI_Comm_dup(comm, newcomm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Comm_dup");
  return ret;
}

int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm *newcomm) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Comm_split(comm, color, key, newcomm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Comm_split(*__fenix_g_new_world, color, key, newcomm);
    } else {
      ret = PMPI_Comm_split(comm, color, key, newcomm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Comm_split");
  return ret;
}

int MPI_Alltoallv(void *sendbuf, int *sendcounts, int *sdispls, MPI_Datatype sendtype, 
                  void *recvbuf, int *recvcounts, int *rdispls, MPI_Datatype recvtype,
                  MPI_Comm comm) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = 
      PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf,
                     recvcounts, rdispls, recvtype, comm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf,
                           recvcounts, rdispls, recvtype, *__fenix_g_new_world);
    } else {
      ret = PMPI_Alltoallv(sendbuf, sendcounts, sdispls, sendtype, recvbuf,
                           recvcounts,  rdispls, recvtype, comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Alltoallv");
  return ret;
}

int MPI_Allgather(void *sendbuf, int sendcount, MPI_Datatype sendtype,
                  void *recvbuf, int recvcount, MPI_Datatype recvtype,
                  MPI_Comm comm) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = 
      PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, comm);
  else {
    PMPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, 
                           *__fenix_g_new_world);
    } else {
      ret = PMPI_Allgather(sendbuf, sendcount, sendtype, recvbuf, recvcount, recvtype, 
                           comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Allgather");
  return ret;
}

int MPI_Comm_rank(MPI_Comm comm, int *rank) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Comm_rank(comm, rank);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Comm_rank(*__fenix_g_new_world, rank);
    } else {
      ret = PMPI_Comm_rank(comm, rank);
    }
  }
  __fenix_test_MPI(ret, "MPI_Comm_rank");
  return ret;
}

int MPI_Allreduce(MPI_SEND_BUFF_TYPE sendbuf, void *recvbuf, int count, MPI_Datatype type,
                  MPI_Op op, MPI_Comm comm) {
  int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Allreduce(sendbuf, recvbuf, count, type, op, comm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if( __fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT ) {
      ret = PMPI_Allreduce(sendbuf, recvbuf, count, type, op, *__fenix_g_new_world);
    } else {
      ret = PMPI_Allreduce(sendbuf, recvbuf, count, type, op, comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Allreduce");
  return ret;
}

int MPI_Reduce(MPI_SEND_BUFF_TYPE sendbuf, void *recvbuf, int count, MPI_Datatype type,
                  MPI_Op op, int root, MPI_Comm comm) {
  int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Reduce(sendbuf, recvbuf, count, type, op, root, comm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if( __fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT ) {
      ret = PMPI_Reduce(sendbuf, recvbuf, count, type, op, root, *__fenix_g_new_world);
    } else {
      ret = PMPI_Reduce(sendbuf, recvbuf, count, type, op, root, comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Reduce");
  return ret;
}

int MPI_Barrier(MPI_Comm comm) {
  int ret, flag;
  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Barrier(comm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Barrier(*__fenix_g_new_world);
    } else {
      ret = PMPI_Barrier(comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Barrier");
  return ret;
}

int MPI_Bcast(void *buf, int count, MPI_Datatype type, int root, MPI_Comm comm) {
  int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Bcast(buf, count, type, root, comm);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Bcast(buf, count, type, root, *__fenix_g_new_world);
    } else {
      ret = PMPI_Bcast(buf, count, type, root, comm);
    }
  }
  __fenix_test_MPI(ret, "MPI_Bcast");
  return ret;
}

int MPI_Irecv(void *buf, int count, MPI_Datatype datatype,
              int source, int tag, MPI_Comm comm, MPI_Request *request) {
  int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Irecv(buf, count, datatype, source, tag, *__fenix_g_new_world, request);
    } else {
      ret = PMPI_Irecv(buf, count, datatype, source, tag, comm, request);
    }
    __fenix_insert_request(request);
  }
  __fenix_test_MPI(ret, "MPI_Irecv");
  return ret;
}

int MPI_Isend(MPI_SEND_BUFF_TYPE buf, int count, MPI_Datatype datatype, int dest,
              int tag, MPI_Comm comm, MPI_Request *request) {
  int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
      ret = PMPI_Isend(buf, count, datatype, dest, tag, *__fenix_g_new_world, request);
    } else {
      ret = PMPI_Isend(buf, count, datatype, dest, tag, comm, request);
    }
    __fenix_insert_request(request);
  }
  __fenix_test_MPI(ret, "MPI_Isend");
  return ret;
}

int MPI_Recv(void *buf, int count, MPI_Datatype type, int source, int tag, MPI_Comm comm,
             MPI_Status *status) {
#if 0
    int ret, flag;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    else {
	MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
	if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
	    ret = PMPI_Recv(buf, count, type, source, tag, *__fenix_g_new_world, status);
	} else {
	    ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
	}
    }
    __fenix_test_MPI(ret, "MPI_Recv");
    return ret;
#elif 0
    int ret;
    ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    return ret;
#elif 0
    int ret;
    ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    __fenix_test_MPI(ret, "MPI_Recv");
    return ret;
#elif 0
    int ret;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    else {
	ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    }
    __fenix_test_MPI(ret, "MPI_Recv");
    return ret;
#elif 0
    int ret, flag;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    else {
	MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
	ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    }
    __fenix_test_MPI(ret, "MPI_Recv");
    return ret;
#elif 1
    int ret, flag;
    /* printf("mpi recv\n"); */
    /* fflush(stdout); */
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
    else {
	if(__fenix_g_replace_comm_flag == 1) {
	    if(comm == __fenix_g_original_comm__) {
		//printf("%p    %p\n", (void *) comm, (void *) __fenix_g_original_comm__);
		/* printf("same!\n"); */
		ret = PMPI_Recv(buf, count, type, source, tag, *__fenix_g_new_world, status);
	    } else {
		MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
		if(flag == MPI_CONGRUENT) {
		    ret = PMPI_Recv(buf, count, type, source, tag, *__fenix_g_new_world, status);
		} else {
		    ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
		}
	    }
	} else {
	    ret = PMPI_Recv(buf, count, type, source, tag, comm, status);
	}
    }
    __fenix_test_MPI(ret, "MPI_Recv");
    return ret;
#endif
}

int MPI_Send(MPI_SEND_BUFF_TYPE buf, int count, MPI_Datatype type, int dest, int tag,
             MPI_Comm comm) {
#if 0
    int ret, flag;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Send(buf, count, type, dest, tag, comm);
    else {
	MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
	if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
	    ret = PMPI_Send(buf, count, type, dest, tag, *__fenix_g_new_world);
	} else {
	    ret = PMPI_Send(buf, count, type, dest, tag, comm);
	}
    }
    __fenix_test_MPI(ret, "MPI_Send");
    return ret;
#elif 0
    int ret;
    ret = PMPI_Send(buf, count, type, dest, tag, comm);
    return ret;
#elif 0
    int ret;
    ret = PMPI_Send(buf, count, type, dest, tag, comm);
    __fenix_test_MPI(ret, "MPI_Send");
    return ret;
#elif 0
    int ret, flag;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Send(buf, count, type, dest, tag, comm);
    else {
	ret = PMPI_Send(buf, count, type, dest, tag, comm);
    }
    __fenix_test_MPI(ret, "MPI_Send");
    return ret;
#elif 0
    int ret, flag;
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Send(buf, count, type, dest, tag, comm);
    else {
	MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
	ret = PMPI_Send(buf, count, type, dest, tag, comm);
    }
    __fenix_test_MPI(ret, "MPI_Send");
    return ret;
#elif 1
    int ret, flag;
    /* printf("mpi send\n"); */
    /* fflush(stdout); */
    if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Send(buf, count, type, dest, tag, comm);
    else {
	if(__fenix_g_replace_comm_flag == 1) {
	    /* printf("%p    %p\n", (void *) comm, (void *) __fenix_g_original_comm__); */
	    /* fflush(stdout); */
	    if(comm == __fenix_g_original_comm__) {
		/* printf("same!!!\n"); */
		ret = PMPI_Send(buf, count, type, dest, tag, *__fenix_g_new_world);
	    } else {
		MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
		if(flag == MPI_CONGRUENT) {
		    ret = PMPI_Send(buf, count, type, dest, tag, *__fenix_g_new_world);
		} else {
		    ret = PMPI_Send(buf, count, type, dest, tag, comm);
		}
	    }
	} else {
		ret = PMPI_Send(buf, count, type, dest, tag, comm);
	}
    }
    __fenix_test_MPI(ret, "MPI_Send");
    return ret;
#endif
}

int MPI_Sendrecv(MPI_SEND_BUFF_TYPE sendbuf, int sendcount, MPI_Datatype sendtype,
                int dest, int sendtag,
                void *recvbuf, int recvcount, MPI_Datatype recvtype,
                int source, int recvtag,
                MPI_Comm comm, MPI_Status *status) {
    int ret, flag;

  if (!(__fenix_g_fenix_init_flag)) ret = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
  else {
    MPI_Comm_compare(comm, __fenix_g_original_comm, &flag);
    if(__fenix_g_replace_comm_flag == 1  &&  flag == MPI_CONGRUENT) {
        ret = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, *__fenix_g_new_world, status);
    } else {
        ret = PMPI_Sendrecv(sendbuf, sendcount, sendtype, dest, sendtag, recvbuf, recvcount, recvtype, source, recvtag, comm, status);
      }
  }
    __fenix_test_MPI(ret, "MPI_Sendrecv");
    return ret;
}

int MPI_Wait(MPI_Request *request, MPI_Status *status) {
  int ret;
  if (!(__fenix_g_fenix_init_flag)) {
    ret = PMPI_Wait(request, status);
  } else {
    ret = PMPI_Wait(request, status);
    __fenix_test_MPI(ret, "MPI_Wait");
    __fenix_remove_request(request);
  }
  return ret;
}
