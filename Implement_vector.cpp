namespace getcracked {

    template <typename T, typename Allocator = std::allocator<T>>
    class vector {
    public:
        vector() : vector(1) {}  // default constructor
    
        explicit vector(size_t capacity) { reserve(capacity); } // constructor with capacity
    
        vector(const vector& other) { copy_from(other);} // vector and spider man (action, spider main is shotting a liquid medication to turn the villain to spider man club)
    
        vector& operator=(const vector& other) {
            if (this == &other) return *this;
            copy_from(other);
            return *this;
        }
    
        vector(vector&& other) noexcept { steal_from(std::move(other)); } // if it breaks it breaks out of the function -> ( not expecting to throw an exception)
    
        vector& operator=(vector&& other) noexcept {
            if (this == &other) return *this;
            destroy_and_deallocate();
            steal_from(std::move(other));
            return *this;
        }
    
        ~vector() { destroy_and_deallocate(); }
    
        bool empty() const { return size_ == 0; }
    
        T& at(size_t index) {
            if (index >= size_) throw std::out_of_range("Index is too large.");
            return data_[index];
        }
    
        const T& at(size_t index) const {
            if (index >= size_) throw std::out_of_range("Index is too large.");
            return data_[index];
        }
    
        void shrink_to_fit() { reserve(size_); }
    
        size_t get_size()     const { return size_; }
        size_t get_capacity() const { return capacity_; }
    
        void reserve(size_t capacity) {
            if (capacity == capacity_) return;
            if (capacity == 0) { destroy_and_deallocate(); return; }
    
            Allocator allocator;
            T* memory = allocator.allocate(capacity);
    
            size_t constructed = 0;
            try {
                for (; constructed < size_; ++constructed) {
                    new (memory + constructed) T{data_[constructed]};
                }
            } catch (...) {
                for (size_t j = 0; j < constructed; ++j) memory[j].~T();
                allocator.deallocate(memory, capacity);
                throw;
            }
    
            if (data_ != nullptr) {
                for (size_t i = 0; i < size_; ++i) data_[i].~T();
                allocator.deallocate(data_, capacity_);
            }
    
            data_ = memory;
            capacity_ = capacity;
        }
    
        void push_back(const T& item) {
            try_expand_capacity();
            new (data_ + size_) T{item};
            ++size_;
        }
    
        template <typename... Args>
        void emplace_back(Args&&... args) {
            try_expand_capacity();
            new (data_ + size_) T(std::forward<Args>(args)...);
            ++size_;
        }
    
        void pop_back() {
            // UB if empty (per problem spec)
            data_[--size_].~T();
        }
    
        void clear() {
            while (size_) pop_back();
        }
    
    private:
        void steal_from(vector&& other) {
            data_     = std::exchange(other.data_, nullptr);
            capacity_ = std::exchange(other.capacity_, 0);
            size_     = std::exchange(other.size_, 0);
        }
    
        void copy_from(const vector& other) {
            Allocator allocator;
            T* memory = (other.capacity_ > 0)
                            ? allocator.allocate(other.capacity_)
                            : nullptr;
    
            size_t constructed = 0;
            try {
                for (; constructed < other.size_; ++constructed) {
                    new (memory + constructed) T{other.data_[constructed]};   // FIXED
                }
            } catch (...) {
                for (size_t j = 0; j < constructed; ++j) memory[j].~T();
                if (memory) allocator.deallocate(memory, other.capacity_);
                throw;
            }
    
            destroy_and_deallocate();
    
            data_ = memory;
            capacity_ = other.capacity_;
            size_ = other.size_;
        }
    
        void destroy_and_deallocate() {
            clear();
            if (data_ != nullptr) {
                Allocator allocator;
                allocator.deallocate(data_, capacity_);
                data_ = nullptr;
                capacity_ = 0;
            }
        }
    
        void try_expand_capacity() {
            if (!should_increase_capacity()) return;
    
            auto capacity = capacity_ == 0 ? size_t{1} : capacity_;
            const auto max_capacity = std::numeric_limits<size_t>::max();
    
            if (max_capacity / capacity_increase_factor() < capacity) {
                reserve(max_capacity);             // FIXED: don't multiply past max
            } else {
                reserve(capacity * capacity_increase_factor());
            }
        }
    
        bool should_increase_capacity() const {
            if (capacity_ == 0) return true;
            return capacity_ / capacity_increase_factor() <= size_;
        }
    
        size_t capacity_increase_factor() const { return 3; }
    
    private:
        size_t size_{};
        size_t capacity_{};
        T* data_{nullptr};
    };
    
    }  // namespace getcracked