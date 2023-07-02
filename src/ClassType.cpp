#include <asgard/codegen/ClassType.h>
#include <asgard/codegen/Declaration.h>
#include <asgard/codegen/Method.h>


namespace asgard{
namespace codegen{


void ClassType::set_parent(std::shared_ptr<Type> new_parent){
	parent = new_parent;
}


}
}

