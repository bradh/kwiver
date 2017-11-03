/*ckwg +29
 * Copyright 2017 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * @file
 * @brief Output operators for KPF canonical types in text format.
 *
 * This file writes KPF canonical types to a stream in a text format.
 * This format is deprecated in favor of YAML, but is retained as an example
 * for adding new formats.
 */

#ifndef KWIVER_VITAL_KPF_TEXT_WRITER_H_
#define KWIVER_VITAL_KPF_TEXT_WRITER_H_

#include <vital/kpf/vital_kpf_export.h>

#include <iostream>
#include <vital/kpf/kpf_canonical_types.h>
#include <vital/kpf/kpf_canonical_io.h>

namespace kwiver {
namespace vital {
namespace kpf {

struct VITAL_KPF_EXPORT private_endl_t
{};

class VITAL_KPF_EXPORT record_text_writer
{
public:
  explicit record_text_writer( std::ostream& os ) : s( os ) {}

  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::id_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::bbox_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::timestamp_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::kv_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::conf_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::poly_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const writer< canonical::meta_t >& io );
  friend VITAL_KPF_EXPORT record_text_writer& operator<<( record_text_writer& w, const private_endl_t& );

  static private_endl_t endl;

private:
  std::ostream& s;
};

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::id_t >& io );

VITAL_KPF_EXPORT
packet_bounce_t& operator>>( packet_bounce_t& w, const writer< canonical::id_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::bbox_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::timestamp_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::kv_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::conf_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::poly_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const writer< canonical::meta_t >& io );

VITAL_KPF_EXPORT
record_text_writer& operator<<( record_text_writer& w, const private_endl_t& e );

} // ...kpf
} // ...vital
} // ...kwiver


#endif