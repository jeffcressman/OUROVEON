//   _______ _______ ______ _______ ___ ___ _______ _______ _______ 
//  |       |   |   |   __ \       |   |   |    ___|       |    |  |
//  |   -   |   |   |      <   -   |   |   |    ___|   -   |       |
//  |_______|_______|___|__|_______|\_____/|_______|_______|__|____|
//  \\ harry denholm \\ ishani            ishani.org/shelf/ouroveon/
//
//  

#include "pch.h"

#include "base/operations.h"

namespace base {

using OperationIDs = mcc::ReaderWriterQueue<OperationID>;

std::unique_ptr< OperationIDs >     gOperationIDCache;
uint32_t                            gOperationIDCounter;
std::mutex                          gOperationIDCacheFillLock;

void OperationsFill( std::size_t count )
{
    std::scoped_lock<std::mutex> fillLock( gOperationIDCacheFillLock );
    for ( auto i = 0; i < count; i++ )
    {
        gOperationIDCache->emplace( gOperationIDCounter++ );
    }
}

// ---------------------------------------------------------------------------------------------------------------------

// private call from app boot-up to one-time initialise the shared ID counter
void OperationsInit()
{
    gOperationIDCache   = std::make_unique<OperationIDs>();
    gOperationIDCounter = OperationID::defaultValue();

    OperationsFill( 1024 );
}
// .. and for symmetry
void OperationsTerm()
{
    gOperationIDCache = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
OperationID Operations::newID( const OperationVariant variant )
{
    ABSL_ASSERT( gOperationIDCache != nullptr );

    // in most cases, this should be a fast & lockfree result; if we just ran out, lock and refill
    // potentially multiple threads could enqueue a refill if they all fail at the same time which is 
    // still technically fine
    OperationID result;
    while ( !gOperationIDCache->try_dequeue( result ) )
    {
        OperationsFill( 256 );
    }

    // encode variant ID
    const uint32_t opValue = result.get();
    const uint32_t finalValue = ( opValue & 0x00FFFFFF ) | ( variant.get() << 24 );

    return OperationID( finalValue );
}

// ---------------------------------------------------------------------------------------------------------------------
OperationVariant Operations::variantFromID( const OperationID operationID )
{
    const uint32_t opValue  = operationID.get();
    const uint32_t varValue = ( opValue & 0xFF000000 ) >> 24;

    return OperationVariant( varValue );
}


} // namespace base
