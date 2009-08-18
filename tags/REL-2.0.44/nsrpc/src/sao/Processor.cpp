#include "stdafx.h"
#include <nsrpc/sao/Processor.h>
#include <nsrpc/sao/Proxy.h>
#include "Scheduler.h"

namespace nsrpc
{

namespace sao
{

Processor::Processor(ProcessorCallback* callback) :
    scheduler_(new Scheduler(callback)),
    proxy_(new Proxy(*scheduler_))
{
}


Processor::~Processor()
{
    finalize();
}


bool Processor::initialize()
{
    return scheduler_->start();
}


void Processor::finalize()
{
    scheduler_->stop();
    scheduler_->flush();
}

} // namespace sao

} // namespace nsrpc
