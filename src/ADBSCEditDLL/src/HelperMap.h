namespace HelperMap
{
	template <typename T, typename U>
	class InitMap
	{
		private:
			std::map<T, U> map_;

		public:
			InitMap(const T & key, const U & val)
			{
				map_[key] = val;
			}
			InitMap<T, U> & operator()(const T & key, const U & val)
			{
				map_[key] = val;
				return *this;
			}
			operator std::map<T, U>()
			{
				return map_;
			}
	};
	typedef std::map<std::string, std::string> StringMap;
}

