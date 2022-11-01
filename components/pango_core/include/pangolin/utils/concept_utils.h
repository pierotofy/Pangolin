#pragma once

namespace pangolin
{

template<class T, class U>
concept DerivedFrom = std::is_base_of<U, T>::value;

}