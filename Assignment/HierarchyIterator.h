#pragma once

#include "libds/amt/explicit_hierarchy.h"
#include <vector>
#include <exception>

template<typename DataType>
class HierarchyIterator {
private:
    ds::amt::ExplicitHierarchyBlock<DataType>* current;

public:
    explicit HierarchyIterator(ds::amt::ExplicitHierarchyBlock<DataType>* rootNode) : current(rootNode) {}

    bool isValid() const {
        return current != nullptr;
    }

    bool moveToParent() {
        if (current && current->parent_) {
            current = current->parent_;
            return true;
        }
        return false;
    }

    // Moves to a specific child node by index -> I am using 'MultiWayExplicitHierarchyBlock'
    bool moveToChild(size_t index) {
        auto* multiNode = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<DataType>*>(current);
        if (multiNode && index < multiNode->sons_->size()) {
            current = (*multiNode->sons_).access(index)->data_;
            return true;
        }
        return false;
    }

    DataType& operator*() const {
        if (!current) {
            throw std::out_of_range("Iterator is out of range");
        }
        return current->data_;
    }

    DataType* operator->() const {
        if (!current) {
            throw std::out_of_range("Iterator is out of range");
        }
        return &(current->data_);
    }

    void reset(ds::amt::ExplicitHierarchyBlock<DataType>* rootNode) {
        current = rootNode;
    }

    ds::amt::ExplicitHierarchyBlock<DataType>* getCurrentNode() const {
		return current;
	}
};
