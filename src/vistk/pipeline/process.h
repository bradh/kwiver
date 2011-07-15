/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef VISTK_PIPELINE_PROCESS_H
#define VISTK_PIPELINE_PROCESS_H

#include "edge.h"
#include "process_registry.h"
#include "types.h"

#include <boost/utility.hpp>

#include <string>
#include <vector>

namespace vistk
{

/// A group of processes.
typedef std::vector<process_t> processes_t;

/**
 * \class process
 *
 * \brief A node within a \ref pipeline which runs computations on data.
 *
 * \ingroup base_classes
 */
class process
  : boost::noncopyable
{
  public:
    /// The type for the name of a process.
    typedef std::string name_t;
    /// The type for the name of a port on a process.
    typedef std::string port_t;
    /// A group of ports.
    typedef std::vector<port_t> ports_t;

    /**
     * \brief Post-connection initialization.
     */
    void init();

    /**
     * \brief Steps through one iteration of the process.
     */
    void step();

    /**
     * \brief Connects an edge to an input port on the process.
     *
     * \param port The port to connect to.
     * \param edge The edge to connect to the port.
     */
    void connect_input_port(port_t const& port, edge_t edge);
    /**
     * \brief Connects an edge to an output port on the process.
     *
     * \throws null_edge_port_connection Thrown when \p edge is \c NULL.
     *
     * \param port The port to connect to.
     * \param edge The edge to connect to the port.
     */
    void connect_output_port(port_t const& port, edge_t edge);

    /**
     * \brief A list of input ports available on the process.
     */
    ports_t input_ports() const;
    /**
     * \brief A list of output ports available on the process.
     */
    ports_t output_ports() const;

    /**
     * \brief The name of the process.
     */
    name_t name() const;
    /**
     * \brief The type of the process.
     */
    virtual process_registry::type_t type() const = 0;
  protected:
    /**
     * \brief Constructor.
     *
     * \param config Contains configuration for the process.
     */
    process(config_t const& config);
    /**
     * \brief Destructor.
     */
    virtual ~process();

    /**
     * \brief Initialization checks for subclasses.
     */
    virtual void _init();

    /**
     * \brief Method where subclass data processing occurs.
     */
    virtual void _step();

    /**
     * \brief Subclass input connection method.
     *
     * \param port The port to connect to.
     * \param edge The edge to connect to the port.
     */
    virtual void _connect_input_port(port_t const& port, edge_t edge);
    /**
     * \brief Subclass output connection method.
     *
     * \param port The port to connect to.
     * \param edge The edge to connect to the port.
     */
    virtual void _connect_output_port(port_t const& port, edge_t edge);

    /**
     * \brief Subclass input ports.
     */
    virtual ports_t _input_ports() const;
    /**
     * \brief Subclass output ports.
     */
    virtual ports_t _output_ports() const;

    /**
     * \brief Marks the process as complete.
     */
    void mark_as_complete();
    /**
     * \brief The \ref stamp that the hearbeat is based off of.
     */
    stamp_t heartbeat_stamp() const;
    /**
     * \brief Check if a set of edges carry the same colored data.
     *
     * \param edges The edges to check the color of.
     */
    static bool same_colored_edges(edges_t const& edges);
    /**
     * \brief Check if a set of edges are syncronized.
     *
     * Makes sure that the given edges all have data that carry equivalent
     * stamps.
     *
     * \param edges The edges to check.
     */
    static bool sync_edges(edges_t const& edges);
  private:
    class priv;
    boost::shared_ptr<priv> d;
};

} // end namespace vistk

#endif // VISTK_PIPELINE_PROCESS_H
