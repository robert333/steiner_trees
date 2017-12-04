#ifndef MIP_VARIABLESTORAGE_HPP
#define MIP_VARIABLESTORAGE_HPP

#include "../Storage.hpp"
#include "MIPModel.hpp"

namespace mip {

template<typename ... Keys>
class VariableStorage : public Storage<MIPModel::Variable*, Keys ...> {
public:
	explicit VariableStorage() = default;

	Value solution_value(Keys ... keys) const;
};

template<typename ... Keys>
Value VariableStorage<Keys ...>::solution_value(Keys ... keys) const
{
	return this->get(keys ...)->solution_value();
}

} // namespace mip

#endif
