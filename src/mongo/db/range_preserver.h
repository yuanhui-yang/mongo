/**
 *    Copyright (C) 2013 10gen Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#include <boost/optional.hpp>

#include "mongo/db/catalog/collection.h"
#include "mongo/db/clientcursor.h"

namespace mongo {

/**
 * A RangePreserver prevents the RangeDeleter from removing any new data ranges in a collection.
 * Previously queued ranges may still be deleted but the documents in those ranges will be
 * filtered by CollectionMetadata::belongsToMe.
 */
class RangePreserver {
public:
    /**
     * Sharding uses the set of active cursor IDs as the current state. We pin a dummy
     * ClientCursor, which creates an additional cursor ID. The cursor ID lasts as long as this
     * object does. The ClientCursorPin guarantees that the underlying ClientCursor is not deleted
     * until this object goes out of scope.
     */
    RangePreserver(const Collection* collection) {
        // Empty collections don't have any data we need to preserve
        if (collection) {
            // Pin keeps the CC from being deleted while it's in scope.  We delete it ourselves.
            _pin.emplace(collection->getCursorManager()->registerRangePreserverCursor(collection));
        }
    }

    void release() {
        if (_pin) {
            _pin->deleteUnderlying();
            _pin.reset();
        }
    }

    ~RangePreserver() {
        release();
    }

private:
    boost::optional<ClientCursorPin> _pin;
};

}  // namespace mongo