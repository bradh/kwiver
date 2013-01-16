/*ckwg +5
 * Copyright 2011-2012 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <python/helpers/pystream.h>
#include <python/helpers/python_convert_optional.h>

#include <vistk/pipeline_util/load_pipe.h>
#include <vistk/pipeline_util/load_pipe_exception.h>
#include <vistk/pipeline_util/pipe_declaration_types.h>

#include <vistk/pipeline/process.h>

#include <vistk/utilities/path.h>

#include <vistk/python/util/python_gil.h>

#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/class.hpp>
#include <boost/python/def.hpp>
#include <boost/python/module.hpp>
#include <boost/python/exception_translator.hpp>

#include <string>

/**
 * \file load.cxx
 *
 * \brief Python bindings for loading pipe blocks.
 */

using namespace boost::python;

static object pipe_block_config(vistk::pipe_block const& block);
static void pipe_block_config_set(vistk::pipe_block& block, vistk::config_pipe_block const& config);
static object pipe_block_process(vistk::pipe_block const& block);
static void pipe_block_process_set(vistk::pipe_block& block, vistk::process_pipe_block const& process);
static object pipe_block_connect(vistk::pipe_block const& block);
static void pipe_block_connect_set(vistk::pipe_block& block, vistk::connect_pipe_block const& connect);
static object cluster_subblock_config(vistk::cluster_subblock_t const& subblock);
static void cluster_subblock_config_set(vistk::cluster_subblock_t& subblock, vistk::cluster_config_t const& config);
static object cluster_subblock_input(vistk::cluster_subblock_t const& subblock);
static void cluster_subblock_input_set(vistk::cluster_subblock_t& subblock, vistk::cluster_input_t const& input);
static object cluster_subblock_output(vistk::cluster_subblock_t const& subblock);
static void cluster_subblock_output_set(vistk::cluster_subblock_t& subblock, vistk::cluster_output_t const& output);
static object cluster_block_config(vistk::cluster_block const& block);
static void cluster_block_config_set(vistk::cluster_block& block, vistk::config_pipe_block const& config);
static object cluster_block_process(vistk::cluster_block const& block);
static void cluster_block_process_set(vistk::cluster_block& block, vistk::process_pipe_block const& process);
static object cluster_block_connect(vistk::cluster_block const& block);
static void cluster_block_connect_set(vistk::cluster_block& block, vistk::connect_pipe_block const& connect);
static object cluster_block_cluster(vistk::cluster_block const& block);
static void cluster_block_cluster_set(vistk::cluster_block& block, vistk::cluster_pipe_block const& cluster);
static vistk::pipe_blocks load_pipe_file(std::string const& path);
static vistk::pipe_blocks load_pipe(object const& stream, std::string const& inc_root);
static vistk::cluster_blocks load_cluster_file(std::string const& path);
static vistk::cluster_blocks load_cluster(object const& stream, std::string const& inc_root);

BOOST_PYTHON_MODULE(load)
{
  register_optional_converter<vistk::config_flags_t>("ConfigFlagsOpt", "An optional config flags.");
  register_optional_converter<vistk::config_provider_t>("ConfigProviderOpt", "An optional config provider.");
  register_optional_converter<vistk::process::port_flags_t>("PortFlagsOpt", "An optional port flags.");

  class_<vistk::token_t>("Token"
    , "A token in the pipeline description.");
  class_<vistk::config_flag_t>("ConfigFlag"
    , "A flag on a configuration setting.");
  class_<vistk::config_flags_t>("ConfigFlags"
    , "A collection of flags on a configuration setting.")
    .def(vector_indexing_suite<vistk::config_flags_t>())
  ;
  class_<vistk::config_provider_t>("ConfigProvider"
    , "A provider key for a configuration setting.");
  class_<vistk::config_key_options_t>("ConfigKeyOptions"
    , "A collection of options on a configuration setting.")
    .def_readwrite("flags", &vistk::config_key_options_t::flags)
    .def_readwrite("provider", &vistk::config_key_options_t::provider)
  ;
  class_<vistk::config_key_t>("ConfigKey"
    , "A configuration key with its settings.")
    .def_readwrite("key_path", &vistk::config_key_t::key_path)
    .def_readwrite("options", &vistk::config_key_t::options)
  ;
  class_<vistk::config_value_t>("ConfigValue"
    , "A complete configuration setting.")
    .def_readwrite("key", &vistk::config_value_t::key)
    .def_readwrite("value", &vistk::config_value_t::value)
  ;
  class_<vistk::config_values_t>("ConfigValues"
    , "A collection of configuration settings.")
    /// \todo Need operator == on config_value_t
    //.def(vector_indexing_suite<vistk::config_values_t>())
  ;
  class_<vistk::config_pipe_block>("ConfigBlock"
    , "A block of configuration settings.")
    .def_readwrite("key", &vistk::config_pipe_block::key)
    .def_readwrite("values", &vistk::config_pipe_block::values)
  ;
  class_<vistk::process_pipe_block>("ProcessBlock"
    , "A block which declares a process.")
    .def_readwrite("name", &vistk::process_pipe_block::name)
    .def_readwrite("type", &vistk::process_pipe_block::type)
    .def_readwrite("config_values", &vistk::process_pipe_block::config_values)
  ;
  class_<vistk::connect_pipe_block>("ConnectBlock"
    , "A block which connects two ports together.")
    .def_readwrite("from_", &vistk::connect_pipe_block::from)
    .def_readwrite("to", &vistk::connect_pipe_block::to)
  ;
  class_<vistk::pipe_block>("PipeBlock"
    , "A block in a pipeline declaration file.")
    .add_property("config", &pipe_block_config, &pipe_block_config_set)
    .add_property("process", &pipe_block_process, &pipe_block_process_set)
    .add_property("connect", &pipe_block_connect, &pipe_block_connect_set)
  ;
  class_<vistk::pipe_blocks>("PipeBlocks"
    , "A collection of pipeline blocks.")
    /// \todo Need operator == on pipe_block.
    //.def(vector_indexing_suite<vistk::pipe_blocks>())
  ;
  class_<vistk::cluster_config_t>("ClusterConfig"
    , "A configuration value for a cluster.")
    .def_readwrite("description", &vistk::cluster_config_t::description)
    .def_readwrite("config_value", &vistk::cluster_config_t::config_value)
  ;
  class_<vistk::cluster_input_t>("ClusterInput"
    , "An input mapping for a cluster.")
    .def_readwrite("description", &vistk::cluster_input_t::description)
    .def_readwrite("from_", &vistk::cluster_input_t::from)
    .def_readwrite("targets", &vistk::cluster_input_t::targets)
  ;
  class_<vistk::cluster_output_t>("ClusterOutput"
    , "An output mapping for a cluster.")
    .def_readwrite("description", &vistk::cluster_output_t::description)
    .def_readwrite("from_", &vistk::cluster_output_t::from)
    .def_readwrite("to", &vistk::cluster_output_t::to)
  ;
  class_<vistk::cluster_subblock_t>("ClusterSubblock"
    , "A subblock within a cluster.")
    .add_property("config", &cluster_subblock_config, &cluster_subblock_config_set)
    .add_property("input", &cluster_subblock_input, &cluster_subblock_input_set)
    .add_property("output", &cluster_subblock_output, &cluster_subblock_output_set)
  ;
  class_<vistk::cluster_subblocks_t>("ClusterSubblocks"
    , "A collection of cluster subblocks.")
    /// \todo Need operator == on cluster_subblock_t.
    //.def(vector_indexing_suite<vistk::cluster_subblocks_t>())
  ;
  class_<vistk::cluster_pipe_block>("ClusterBlock"
    , "A block which declares a cluster within the pipeline.")
    .def_readwrite("type", &vistk::cluster_pipe_block::type)
    .def_readwrite("description", &vistk::cluster_pipe_block::description)
    .def_readwrite("subblocks", &vistk::cluster_pipe_block::subblocks)
  ;
  class_<vistk::cluster_block>("ClusterDefineBlock"
    , "A block in a pipeline declaration file.")
    .add_property("config", &cluster_block_config, &cluster_block_config_set)
    .add_property("process", &cluster_block_process, &cluster_block_process_set)
    .add_property("connect", &cluster_block_connect, &cluster_block_connect_set)
    .add_property("cluster", &cluster_block_cluster, &cluster_block_cluster_set)
  ;
  class_<vistk::cluster_blocks>("ClusterDefineBlocks"
    , "A collection of cluster blocks.")
    /// \todo Need operator == on cluster_block.
    //.def(vector_indexing_suite<vistk::cluster_blocks>())
  ;

  def("load_pipe_file", &load_pipe_file
    , (arg("path"))
    , "Load pipe blocks from a file.");
  def("load_pipe", &load_pipe
    , (arg("stream"), arg("inc_root") = std::string())
    , "Load pipe blocks from a stream.");
  def("load_cluster_file", &load_cluster_file
    , (arg("path"))
    , "Load cluster blocks from a file.");
  def("load_cluster", &load_cluster
    , (arg("stream"), arg("inc_root") = std::string())
    , "Load cluster blocks from a stream.");
}

class pipe_block_visitor
  : public boost::static_visitor<object>
{
  public:
    typedef enum
    {
      BLOCK_CONFIG,
      BLOCK_PROCESS,
      BLOCK_CONNECT,
      BLOCK_CLUSTER
    } block_t;

    pipe_block_visitor(block_t type);
    ~pipe_block_visitor();

    block_t const block_type;

    object operator () (vistk::config_pipe_block const& config_block) const;
    object operator () (vistk::process_pipe_block const& process_block) const;
    object operator () (vistk::connect_pipe_block const& connect_block) const;
    object operator () (vistk::cluster_pipe_block const& cluster_block) const;
};

object
pipe_block_config(vistk::pipe_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_CONFIG), block);
}

void
pipe_block_config_set(vistk::pipe_block& block, vistk::config_pipe_block const& config)
{
  block = config;
}

object
pipe_block_process(vistk::pipe_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_PROCESS), block);
}

void
pipe_block_process_set(vistk::pipe_block& block, vistk::process_pipe_block const& process)
{
  block = process;
}

object
pipe_block_connect(vistk::pipe_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_CONNECT), block);
}

void
pipe_block_connect_set(vistk::pipe_block& block, vistk::connect_pipe_block const& connect)
{
  block = connect;
}

class cluster_subblock_visitor
  : public boost::static_visitor<object>
{
  public:
    typedef enum
    {
      BLOCK_CONFIG,
      BLOCK_INPUT,
      BLOCK_OUTPUT
    } block_t;

    cluster_subblock_visitor(block_t type);
    ~cluster_subblock_visitor();

    block_t const block_type;

    object operator () (vistk::cluster_config_t const& config) const;
    object operator () (vistk::cluster_input_t const& input) const;
    object operator () (vistk::cluster_output_t const& output) const;
};

object
cluster_subblock_config(vistk::cluster_subblock_t const& subblock)
{
  return boost::apply_visitor(cluster_subblock_visitor(cluster_subblock_visitor::BLOCK_CONFIG), subblock);
}

void
cluster_subblock_config_set(vistk::cluster_subblock_t& subblock, vistk::cluster_config_t const& config)
{
  subblock = config;
}

object
cluster_subblock_input(vistk::cluster_subblock_t const& subblock)
{
  return boost::apply_visitor(cluster_subblock_visitor(cluster_subblock_visitor::BLOCK_INPUT), subblock);
}

void
cluster_subblock_input_set(vistk::cluster_subblock_t& subblock, vistk::cluster_input_t const& input)
{
  subblock = input;
}

object
cluster_subblock_output(vistk::cluster_subblock_t const& subblock)
{
  return boost::apply_visitor(cluster_subblock_visitor(cluster_subblock_visitor::BLOCK_OUTPUT), subblock);
}

void
cluster_subblock_output_set(vistk::cluster_subblock_t& subblock, vistk::cluster_output_t const& output)
{
  subblock = output;
}

object
cluster_block_config(vistk::cluster_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_CONFIG), block);
}

void
cluster_block_config_set(vistk::cluster_block& block, vistk::config_pipe_block const& config)
{
  block = config;
}

object
cluster_block_process(vistk::cluster_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_PROCESS), block);
}

void
cluster_block_process_set(vistk::cluster_block& block, vistk::process_pipe_block const& process)
{
  block = process;
}

object
cluster_block_connect(vistk::cluster_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_CONNECT), block);
}

void
cluster_block_connect_set(vistk::cluster_block& block, vistk::connect_pipe_block const& connect)
{
  block = connect;
}

object
cluster_block_cluster(vistk::cluster_block const& block)
{
  return boost::apply_visitor(pipe_block_visitor(pipe_block_visitor::BLOCK_CLUSTER), block);
}

void
cluster_block_cluster_set(vistk::cluster_block& block, vistk::cluster_pipe_block const& cluster)
{
  block = cluster;
}

vistk::pipe_blocks
load_pipe_file(std::string const& path)
{
  return vistk::load_pipe_blocks_from_file(vistk::path_t(path));
}

vistk::pipe_blocks
load_pipe(object const& stream, std::string const& inc_root)
{
  pyistream istr(stream);

  return vistk::load_pipe_blocks(istr, vistk::path_t(inc_root));
}

vistk::cluster_blocks
load_cluster_file(std::string const& path)
{
  return vistk::load_cluster_blocks_from_file(vistk::path_t(path));
}

vistk::cluster_blocks
load_cluster(object const& stream, std::string const& inc_root)
{
  pyistream istr(stream);

  return vistk::load_cluster_blocks(istr, vistk::path_t(inc_root));
}

pipe_block_visitor
::pipe_block_visitor(block_t type)
  : block_type(type)
{
}

pipe_block_visitor
::~pipe_block_visitor()
{
}

object
pipe_block_visitor
::operator () (vistk::config_pipe_block const& config_block) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  object obj;

  if (block_type == BLOCK_CONFIG)
  {
    obj = object(config_block);
  }

  return obj;
}

object
pipe_block_visitor
::operator () (vistk::process_pipe_block const& process_block) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  object obj;

  if (block_type == BLOCK_PROCESS)
  {
    obj = object(process_block);
  }

  return obj;
}

object
pipe_block_visitor
::operator () (vistk::connect_pipe_block const& connect_block) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  object obj;

  if (block_type == BLOCK_CONNECT)
  {
    obj = object(connect_block);
  }

  return obj;
}

object
pipe_block_visitor
::operator () (vistk::cluster_pipe_block const& cluster_block) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  object obj;

  if (block_type == BLOCK_CLUSTER)
  {
    obj = object(cluster_block);
  }

  return obj;
}

cluster_subblock_visitor
::cluster_subblock_visitor(block_t type)
  : block_type(type)
{
}

cluster_subblock_visitor
::~cluster_subblock_visitor()
{
}

object
cluster_subblock_visitor
::operator () (vistk::cluster_config_t const& config) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  if (block_type == BLOCK_CONFIG)
  {
    return object(config);
  }

  return object();
}

object
cluster_subblock_visitor
::operator () (vistk::cluster_input_t const& input) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  if (block_type == BLOCK_INPUT)
  {
    return object(input);
  }

  return object();
}

object
cluster_subblock_visitor
::operator () (vistk::cluster_output_t const& output) const
{
  vistk::python::python_gil const gil;

  (void)gil;

  if (block_type == BLOCK_OUTPUT)
  {
    return object(output);
  }

  return object();
}
