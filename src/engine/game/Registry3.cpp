	template<typename Args, typename Action>
	void executeOne(const Action& _action)
	{
		if (componentMap.contains(typeid(Args).name()))
		{
			auto& compStruct = componentMap[typeid(Args).name()];

			for (int i = 0; i < compStruct.entities.size(); i++)
			{
				_action(getComponentUnsafe<Args>(compStruct.entities[i]));
			}
		}
	}
	template<typename... Args, typename Action>
	void execute(const Action& _action)
	{
		if constexpr (sizeof...(Args) == 1) 
		{
			executeOne<Args...>(_action);
		}
		if constexpr (sizeof...(Args) == 2) 
		{
			std::tuple<Args...> saved_args;
			
			auto type0 = std::get<0>(saved_args);
			auto type1 = std::get<1>(saved_args);
			
			for (int i = 0; i < 8; i++) 
			{
				auto t = std::make_tuple(
					getComponentUnsafe<decltype(type0)>(componentMap[typeid(type0).name()].entities[i]),
					getComponentUnsafe<decltype(type1)>(componentMap[typeid(type1).name()].entities[i])
				);
				std::apply([&](auto&&... args) {((_action(args...))); }, t);
			}
		}
		if constexpr (sizeof...(Args) == 3) 
		{
			std::tuple<Args...> saved_args;

			auto type0 = std::get<0>(saved_args);
			auto type1 = std::get<1>(saved_args);
			auto type2 = std::get<2>(saved_args);

			for (int i = 0; i < 8; i++) 
			{
				auto t = std::make_tuple(
					getComponentUnsafe<decltype(type0)>(componentMap[typeid(type0).name()].entities[i]),
					getComponentUnsafe<decltype(type1)>(componentMap[typeid(type1).name()].entities[i]),
					getComponentUnsafe<decltype(type2)>(componentMap[typeid(type2).name()].entities[i])
				);
				std::apply([&](auto&&... args) {((_action(args...))); }, t);
			}
		}
	}
