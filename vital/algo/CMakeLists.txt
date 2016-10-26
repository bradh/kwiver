###
# Build file for abstract algorithms
#

# ==================================================================
###
# These headers are installed
#
set( public_headers

  algorithm.h
  analyze_tracks.h
  bundle_adjust.h
  close_loops.h
  compute_ref_homography.h
  convert_image.h
  detected_object_set_input.h
  detected_object_set_output.h
  detect_features.h
  detection_refiner.h
  draw_tracks.h
  estimate_canonical_transform.h
  estimate_essential_matrix.h
  estimate_fundamental_matrix.h
  estimate_homography.h
  estimate_similarity_transform.h
  extract_descriptors.h
  filter_features.h
  geo_map.h
  image_filter.h
  image_io.h
  image_object_detector.h
  initialize_cameras_landmarks.h
  match_features.h
  optimize_cameras.h
  track_features.h
  triangulate_landmarks.h
  video_input.h
  )

set( sources

  algorithm.cxx
  analyze_tracks.cxx
  bundle_adjust.cxx
  close_loops.cxx
  compute_ref_homography.cxx
  convert_image.cxx
  detected_object_set_input.cxx
  detected_object_set_output.cxx
  detect_features.cxx
  detection_refiner.cxx
  draw_tracks.cxx
  estimate_canonical_transform.cxx
  estimate_essential_matrix.cxx
  estimate_fundamental_matrix.cxx
  estimate_homography.cxx
  estimate_similarity_transform.cxx
  extract_descriptors.cxx
  filter_features.cxx
  geo_map.cxx
  image_filter.cxx
  image_io.cxx
  image_object_detector.cxx
  initialize_cameras_landmarks.cxx
  match_features.cxx
  optimize_cameras.cxx
  track_features.cxx
  triangulate_landmarks.cxx
  video_input.cxx
)

kwiver_install_headers(
  ${public_headers}
  SUBDIR   vital/algo
  )

# install export header
kwiver_install_headers(
  ${CMAKE_CURRENT_BINARY_DIR}/vital_algo_export.h
  SUBDIR   vital/algo
  NOPATH
  )

kwiver_add_library( vital_algo
  ${public_headers}
  ${sources}
  ${CMAKE_CURRENT_BINARY_DIR}/vital_algo_export.h
  )

target_link_libraries( vital_algo
  PRIVATE         kwiversys
                  vital_util
  PUBLIC          vital
                  vital_config
                  vital_logger
  )