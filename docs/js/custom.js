document.addEventListener('DOMContentLoaded', function () {
  // 获取到 Table of Contents 中所有的 h3 元素
  var tocItems = document.querySelectorAll('.toc h3');

  tocItems.forEach(function (item) {
    if (item.textContent.includes('CNetwork类方法 - add_edge_from_tuple')) {
      item.textContent = 'add_edge_from_tuple';
    }
  });
});
