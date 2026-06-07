// Copyright 2022 NNTU-CS
#include <algorithm>
#include <functional>
#include <vector>
#include "tree.h"

PMTree::PMTree(const std::vector<char>& src) : base(src) {
    top = new Node(0);
}

PMTree::~PMTree() {
    destroy(top);
}

PMTree::Node* PMTree::generate(const std::vector<char>& rest) {
    Node* cur = new Node(0);
    if (rest.empty()) return cur;

    std::vector<char> sorted_rest = rest;
    std::sort(sorted_rest.begin(), sorted_rest.end());

    for (size_t i = 0; i < sorted_rest.size(); ++i) {
        if (i > 0 && sorted_rest[i] == sorted_rest[i - 1]) continue;
        std::vector<char> leftover = sorted_rest;
        leftover.erase(leftover.begin() + static_cast<std::vector<char>::difference_type>(i));
        Node* child = generate(leftover);
        child->sym = sorted_rest[i];
        cur->links.push_back(child);
    }
    return cur;
}

void PMTree::destroy(Node* ptr) {
    if (!ptr) return;
    for (auto child : ptr->links) {
        destroy(child);
    }
    delete ptr;
}

void traverse(PMTree::Node* cur, std::vector<char>& buf,
    std::vector<std::vector<char>>& out, int level, int limit) {
    if (level == limit) {
        out.push_back(buf);
        return;
    }
    for (auto nxt : cur->links) {
        buf.push_back(nxt->sym);
        traverse(nxt, buf, out, level + 1, limit);
        buf.pop_back();
    }
}

std::vector<std::vector<char>> getAllPerms(PMTree& obj) {
    std::vector<std::vector<char>> out;
    std::vector<char> sorted_src = obj.base;
    std::sort(sorted_src.begin(), sorted_src.end());

    std::vector<char> cur;
    std::vector<bool> used(sorted_src.size(), false);

    std::function<void()> dfs = [&]() {
        if (cur.size() == sorted_src.size()) {
            out.push_back(cur);
            return;
        }
        for (size_t i = 0; i < sorted_src.size(); ++i) {
            if (used[i]) continue;
            if (i > 0 && sorted_src[i] == sorted_src[i - 1] && !used[i - 1]) continue;
            used[i] = true;
            cur.push_back(sorted_src[i]);
            dfs();
            cur.pop_back();
            used[i] = false;
        }
    };

    dfs();
    return out;
}

std::vector<char> getPerm1(PMTree& obj, int pos) {
    auto full = getAllPerms(obj);
    if (pos <= 0 || pos > static_cast<int>(full.size())) return {};
    return full[pos - 1];
}

size_t fact(int n) {
    size_t res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

std::vector<char> getPerm2(PMTree& obj, int pos) {
    std::vector<char> a = obj.base;
    std::sort(a.begin(), a.end());

    int n = static_cast<int>(a.size());
    if (pos <= 0 || static_cast<size_t>(pos) > fact(n)) return {};

    size_t k = static_cast<size_t>(pos - 1);
    std::vector<char> ans;

    for (int i = n; i > 0; --i) {
        size_t block = fact(i - 1);
        size_t idx = k / block;
        k %= block;
        if (idx >= a.size()) return {};
        ans.push_back(a[idx]);
        a.erase(a.begin() + static_cast<std::vector<char>::difference_type>(idx));
    }

    return ans;
}
