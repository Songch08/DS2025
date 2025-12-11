#define NOMINMAX        // 告诉 Windows.h 别定义 min/max 宏
#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iterator> 
#include <chrono> 
#include <windows.h> 

// 1. 排序算法

// 1.1快速排序
template <typename T>
int partition(std::vector<T>& arr, int low, int high) {
    T pivot = arr[high];  // 选取最后一个元素作为枢轴
    int i = low - 1;  // i 是较小元素的索引
    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            std::swap(arr[i], arr[j]);  // 将较小元素与 arr[i] 交换
        }
    }
    std::swap(arr[i + 1], arr[high]);  // 将枢轴元素移到正确位置
    return i + 1;
}

template <typename T>
void quicksort(std::vector<T>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);  // 获取分区点
        quicksort(arr, low, pi - 1);  // 对左边子数组递归排序
        quicksort(arr, pi + 1, high);  // 对右边子数组递归排序
    }
}

// 1.2归并排序
template <typename Iter, typename OutIter>
void merge_sort_impl(Iter first, Iter last, OutIter tmp);

// 合并两个有序区间 [first1,last1) 和 [first2,last2) 到 dst
template <typename Iter1, typename Iter2, typename OutIter>
void my_merge(Iter1 first1, Iter1 last1,
              Iter2 first2, Iter2 last2,
              OutIter dst)
{
    while (first1 != last1 && first2 != last2)
        *dst++ = (*first1 < *first2) ? *first1++ : *first2++;
    std::copy(first1, last1, dst);
    std::copy(first2, last2, dst);
}

// 递归实现
template <typename Iter, typename OutIter>
void merge_sort_impl(Iter first, Iter last, OutIter tmp)
{
    auto n = std::distance(first, last);
    if (n < 2) return;

    auto mid  = first + n / 2;
    OutIter tmp_mid = tmp + (mid - first);

    merge_sort_impl(first, mid, tmp);      // 左半边
    merge_sort_impl(mid, last, tmp_mid);   // 右半边

    my_merge(first, mid, mid, last, tmp);  // 合并到临时区
    std::copy(tmp, tmp + n, first);        // 拷回原容器
}

// 用户接口
template <typename T>
void merge_sort(std::vector<T>& arr)
{
    if (arr.size() < 2) return;
    std::vector<T> tmp(arr.size());
    merge_sort_impl(arr.begin(), arr.end(), tmp.begin());
}
//===================================================
// 1.3起泡排序
template <typename T>
void bubble_sort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// 1.4插入排序
template <typename T>
void insertion_sort(std::vector<T>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 2. NMS算法（非极大值抑制）
struct Box {
    int x, y, w, h;
    float confidence;
};

bool compare(Box& b1, Box& b2) {
    return b1.confidence > b2.confidence;
}

float calculate_iou(Box& box1, Box& box2) {
    int x1 = std::max(box1.x, box2.x);
    int y1 = std::max(box1.y, box2.y);
    int x2 = std::min(box1.x + box1.w, box2.x + box2.w);
    int y2 = std::min(box1.y + box1.h, box2.y + box2.h);

    int inter_area = std::max(0, x2 - x1) * std::max(0, y2 - y1);
    int area1 = box1.w * box1.h;
    int area2 = box2.w * box2.h;

    return (float)inter_area / (area1 + area2 - inter_area);
}

std::vector<Box> nms(std::vector<Box>& boxes, float overlap_thresh = 0.5) {
    std::sort(boxes.begin(), boxes.end(), compare);

    std::vector<Box> selected_boxes;
    while (!boxes.empty()) {
        Box current = boxes.front();
        selected_boxes.push_back(current);
        boxes.erase(boxes.begin());

        std::vector<Box> remaining_boxes;
        for (auto& box : boxes) {
            if (calculate_iou(current, box) < overlap_thresh) {
                remaining_boxes.push_back(box);
            }
        }
        boxes = remaining_boxes;
    }
    return selected_boxes;
}

// 3. 数据生成
std::vector<Box> generate_data(int num_boxes, bool clustered = false) {
    std::vector<Box> boxes;
    for (int i = 0; i < num_boxes; ++i) {
        Box box;
        box.x = rand() % 1000;
        box.y = rand() % 1000;
        box.w = rand() % 80 + 20;
        box.h = rand() % 80 + 20;
        box.confidence = (float)(rand() % 1000) / 1000.0f;

        if (clustered) {
            box.x += rand() % 50 - 25;
            box.y += rand() % 50 - 25;
        }

        boxes.push_back(box);
    }
    return boxes;
}
// 4. 性能测试（微秒版）
void test_sorting_algorithms() {
    srand(time(0));

    std::vector<int> sizes = {100, 1000, 5000, 10000};
    bool distributions[] = {false, true};

    for (int size : sizes) {
        for (bool dist : distributions) {
            std::cout << "测试 " << (dist ? "聚集" : "随机") << " 分布，"
                      << size << " 个边界框" << std::endl;
            std::vector<Box> data = generate_data(size, dist);

            // NMS
            auto t0 = std::chrono::steady_clock::now();
            std::vector<Box> nms_result = nms(data);
            auto t1 = std::chrono::steady_clock::now();
            auto ns = std::chrono::duration<double, std::micro>(t1 - t0).count();
            std::cout << "NMS 执行时间: " << ns << " μs" << std::endl;

            // 提取置信度
            std::vector<float> confidences;
            for (const auto& box : data) confidences.push_back(box.confidence);

            // 快速排序
            t0 = std::chrono::steady_clock::now();
            quicksort(confidences, 0, confidences.size() - 1);
            t1 = std::chrono::steady_clock::now();
            ns = std::chrono::duration<double, std::micro>(t1 - t0).count();
            std::cout << "快速排序 执行时间: " << ns << " μs" << std::endl;

            // 归并排序
            t0 = std::chrono::steady_clock::now();
            merge_sort(confidences);
            t1 = std::chrono::steady_clock::now();
            ns = std::chrono::duration<double, std::micro>(t1 - t0).count();
            std::cout << "归并排序 执行时间: " << ns << " μs" << std::endl;

            // 起泡排序
            t0 = std::chrono::steady_clock::now();
            bubble_sort(confidences);
            t1 = std::chrono::steady_clock::now();
            ns = std::chrono::duration<double, std::micro>(t1 - t0).count();
            std::cout << "起泡排序 执行时间: " << ns << " μs" << std::endl;

            // 插入排序
            t0 = std::chrono::steady_clock::now();
            insertion_sort(confidences);
            t1 = std::chrono::steady_clock::now();
            ns = std::chrono::duration<double, std::micro>(t1 - t0).count();
            std::cout << "插入排序 执行时间: " << ns << " μs" << std::endl;
        }
    }
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    test_sorting_algorithms();
    return 0;
}
