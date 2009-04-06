#include "stdafx.h"
#include <srpc/StreamFactory.h>
#include <srpc/detail/VectorStreamBuffer.h>
#include <srpc/RpcTypes.h>
#include <limits>
#include <ctime>

using namespace srpc;

template <class RType>
void run(bool useBitStream, const char* state, RType defvalue)
{
    const int count = 1000000;

    VectorStreamBuffer buffer;

    const srpc::StreamFactory::StreamType streamType = useBitStream ?
        srpc::StreamFactory::stBit : srpc::StreamFactory::stByte;
    boost::scoped_ptr<srpc::OStream> ostream(
        srpc::StreamFactory::createOStream(streamType, buffer));
    boost::scoped_ptr<srpc::IStream> istream(
        srpc::StreamFactory::createIStream(streamType, buffer));

    {
        const clock_t start = clock();
        try {
            for (int i = 0; i < count; ++i) {
                RType value = defvalue;
                value.write(*ostream);
                value.read(*istream);
            }
        }
        catch (const Exception& e) {
            std::cerr << "(Un)marshaling Error: " << e.what() << "\n";
        }

        const Float32 elapsed = Float32(clock() - start) / CLOCKS_PER_SEC;
        std::cout << (useBitStream ? "bit" : "byte") << " stream, " << state <<
            ", " << elapsed << "\n";
    }
}


int main(int /*argc*/, char* /*argv*/[])
{
    run<RpcIntType<Int8, 3> >(true, "3 bits(signed)", -3);
    run<RpcUIntType<Int8, 3> >(true, "3 bits(unsigned)", 2);

    run<RInt8>(true, "8 bits(signed)", SCHAR_MIN);
    run<RUInt8>(true, "8 bits(unsigned)", UCHAR_MAX);

    run<RInt16>(true, "16 bits(signed)", SCHAR_MIN);
    run<RUInt16>(true, "16 bits(unsigned)", UCHAR_MAX);

    run<RInt32>(true, "32 bits(signed)", SCHAR_MIN);
    run<RUInt32>(true, "32 bits(unsigned)", UCHAR_MAX);

    run<RpcIntType<Int8, 3> >(false, "3 bits(signed)", -3);
    run<RpcUIntType<Int8, 3> >(false, "3 bits(unsigned)", 2);

    run<RInt8>(false, "8 bits(signed)", SCHAR_MIN);
    run<RUInt8>(false, "8 bits(unsigned)", UCHAR_MAX);

    run<RInt16>(false, "16 bits(signed)", SCHAR_MIN);
    run<RUInt16>(false, "16 bits(unsigned)", UCHAR_MAX);

    run<RInt32>(false, "32 bits(signed)", SCHAR_MIN);
    run<RUInt32>(false, "32 bits(unsigned)", UCHAR_MAX);

    return 0;
}
