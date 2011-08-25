/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "registration.h"

#include "sync_schedule.h"
#include "thread_per_process_schedule.h"
#include "thread_pool_schedule.h"

#include <vistk/pipeline/config.h>
#include <vistk/pipeline/schedule_registry.h>

#include <boost/make_shared.hpp>

using namespace vistk;

static schedule_t create_sync_schedule(config_t const& config, pipeline_t const& pipe);
static schedule_t create_thread_per_process_schedule(config_t const& config, pipeline_t const& pipe);
static schedule_t create_thread_pool_schedule(config_t const& config, pipeline_t const& pipe);

void
register_schedules()
{
  static schedule_registry::module_t const module_name = schedule_registry::module_t("example_schedules");

  schedule_registry_t const registry = schedule_registry::self();

  if (registry->is_module_loaded(module_name))
  {
    return;
  }

  registry->register_schedule("sync", "Runs the pipeline synchronously", create_sync_schedule);
  registry->register_schedule("thread_per_process", "Runs each process in its own thread", create_thread_per_process_schedule);
  registry->register_schedule("thread_pool", "Uses a pool of threads to step processes", create_thread_pool_schedule);

  registry->mark_module_as_loaded(module_name);
}

schedule_t
create_sync_schedule(config_t const& config, pipeline_t const& pipe)
{
  return boost::make_shared<sync_schedule>(config, pipe);
}

schedule_t
create_thread_per_process_schedule(config_t const& config, pipeline_t const& pipe)
{
  return boost::make_shared<thread_per_process_schedule>(config, pipe);
}

schedule_t
create_thread_pool_schedule(config_t const& config, pipeline_t const& pipe)
{
  size_t const num_threads = config->get_value<size_t>("num_threads", 0);

  return boost::make_shared<thread_pool_schedule>(config, pipe, num_threads);
}
