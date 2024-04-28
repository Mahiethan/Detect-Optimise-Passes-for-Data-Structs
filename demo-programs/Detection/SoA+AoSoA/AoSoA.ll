; ModuleID = 'AoSoA.c'
source_filename = "AoSoA.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.StructureOne = type { [100345 x i32], [203459 x i32], [315243 x i8] }

@.str = private unnamed_addr constant [21 x i8] c"Accessing struct %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [11 x i8] c"Array a: \0A\00", align 1
@.str.2 = private unnamed_addr constant [14 x i8] c"Index %d: %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"Array b: \0A\00", align 1
@.str.4 = private unnamed_addr constant [11 x i8] c"Array c: \0A\00", align 1
@.str.5 = private unnamed_addr constant [14 x i8] c"Index %d: %c\0A\00", align 1
@gs2 = dso_local global ptr null, align 8
@.str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateStructure(ptr noundef %soa, i32 noundef %size, i32 noundef %sizeA, i32 noundef %sizeB, i32 noundef %sizeC) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %sizeA.addr = alloca i32, align 4
  %sizeB.addr = alloca i32, align 4
  %sizeC.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %soa, ptr %soa.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 %sizeA, ptr %sizeA.addr, align 4
  store i32 %sizeB, ptr %sizeB.addr, align 4
  store i32 %sizeC, ptr %sizeC.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc20, %entry
  %0 = load i32, ptr %j, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end22

for.body:                                         ; preds = %for.cond
  store i32 0, ptr %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %2 = load i32, ptr %i, align 4
  %3 = load i32, ptr %sizeA.addr, align 4
  %cmp2 = icmp slt i32 %2, %3
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %4 = load i32, ptr %i, align 4
  %5 = load ptr, ptr %soa.addr, align 8
  %a = getelementptr inbounds %struct.StructureOne, ptr %5, i32 0, i32 0
  %6 = load i32, ptr %i, align 4
  %idxprom = sext i32 %6 to i64
  %arrayidx = getelementptr inbounds [100345 x i32], ptr %a, i64 0, i64 %idxprom
  store i32 %4, ptr %arrayidx, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %7 = load i32, ptr %i, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond1, !llvm.loop !4

for.end:                                          ; preds = %for.cond1
  store i32 0, ptr %i, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc9, %for.end
  %8 = load i32, ptr %i, align 4
  %9 = load i32, ptr %sizeB.addr, align 4
  %cmp5 = icmp slt i32 %8, %9
  br i1 %cmp5, label %for.body6, label %for.end11

for.body6:                                        ; preds = %for.cond4
  %10 = load i32, ptr %i, align 4
  %11 = load ptr, ptr %soa.addr, align 8
  %b = getelementptr inbounds %struct.StructureOne, ptr %11, i32 0, i32 1
  %12 = load i32, ptr %i, align 4
  %idxprom7 = sext i32 %12 to i64
  %arrayidx8 = getelementptr inbounds [203459 x i32], ptr %b, i64 0, i64 %idxprom7
  store i32 %10, ptr %arrayidx8, align 4
  br label %for.inc9

for.inc9:                                         ; preds = %for.body6
  %13 = load i32, ptr %i, align 4
  %inc10 = add nsw i32 %13, 1
  store i32 %inc10, ptr %i, align 4
  br label %for.cond4, !llvm.loop !6

for.end11:                                        ; preds = %for.cond4
  store i32 0, ptr %i, align 4
  br label %for.cond12

for.cond12:                                       ; preds = %for.inc17, %for.end11
  %14 = load i32, ptr %i, align 4
  %15 = load i32, ptr %sizeC.addr, align 4
  %cmp13 = icmp slt i32 %14, %15
  br i1 %cmp13, label %for.body14, label %for.end19

for.body14:                                       ; preds = %for.cond12
  %16 = load i32, ptr %i, align 4
  %conv = trunc i32 %16 to i8
  %17 = load ptr, ptr %soa.addr, align 8
  %c = getelementptr inbounds %struct.StructureOne, ptr %17, i32 0, i32 2
  %18 = load i32, ptr %i, align 4
  %idxprom15 = sext i32 %18 to i64
  %arrayidx16 = getelementptr inbounds [315243 x i8], ptr %c, i64 0, i64 %idxprom15
  store i8 %conv, ptr %arrayidx16, align 1
  br label %for.inc17

for.inc17:                                        ; preds = %for.body14
  %19 = load i32, ptr %i, align 4
  %inc18 = add nsw i32 %19, 1
  store i32 %inc18, ptr %i, align 4
  br label %for.cond12, !llvm.loop !7

for.end19:                                        ; preds = %for.cond12
  br label %for.inc20

for.inc20:                                        ; preds = %for.end19
  %20 = load i32, ptr %j, align 4
  %inc21 = add nsw i32 %20, 1
  store i32 %inc21, ptr %j, align 4
  br label %for.cond, !llvm.loop !8

for.end22:                                        ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @freeStructure(ptr noundef %soa) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  store ptr %soa, ptr %soa.addr, align 8
  %0 = load ptr, ptr %soa.addr, align 8
  call void @free(ptr noundef %0) #5
  ret void
}

; Function Attrs: nounwind
declare dso_local void @free(ptr noundef) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @printStructure(ptr noundef %soa, i32 noundef %size, i32 noundef %sizeA, i32 noundef %sizeB, i32 noundef %sizeC) #0 {
entry:
  %soa.addr = alloca ptr, align 8
  %size.addr = alloca i32, align 4
  %sizeA.addr = alloca i32, align 4
  %sizeB.addr = alloca i32, align 4
  %sizeC.addr = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %soa, ptr %soa.addr, align 8
  store i32 %size, ptr %size.addr, align 4
  store i32 %sizeA, ptr %sizeA.addr, align 4
  store i32 %sizeB, ptr %sizeB.addr, align 4
  store i32 %sizeC, ptr %sizeC.addr, align 4
  store i32 0, ptr %j, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc26, %entry
  %0 = load i32, ptr %j, align 4
  %1 = load i32, ptr %size.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end28

for.body:                                         ; preds = %for.cond
  %2 = load i32, ptr %j, align 4
  %call = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %2)
  %call1 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  store i32 0, ptr %i, align 4
  br label %for.cond2

for.cond2:                                        ; preds = %for.inc, %for.body
  %3 = load i32, ptr %i, align 4
  %4 = load i32, ptr %sizeA.addr, align 4
  %cmp3 = icmp slt i32 %3, %4
  br i1 %cmp3, label %for.body4, label %for.end

for.body4:                                        ; preds = %for.cond2
  %5 = load i32, ptr %i, align 4
  %6 = load ptr, ptr %soa.addr, align 8
  %a = getelementptr inbounds %struct.StructureOne, ptr %6, i32 0, i32 0
  %7 = load i32, ptr %i, align 4
  %idxprom = sext i32 %7 to i64
  %arrayidx = getelementptr inbounds [100345 x i32], ptr %a, i64 0, i64 %idxprom
  %8 = load i32, ptr %arrayidx, align 4
  %call5 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %5, i32 noundef %8)
  br label %for.inc

for.inc:                                          ; preds = %for.body4
  %9 = load i32, ptr %i, align 4
  %inc = add nsw i32 %9, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond2, !llvm.loop !9

for.end:                                          ; preds = %for.cond2
  %call6 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  store i32 0, ptr %i, align 4
  br label %for.cond7

for.cond7:                                        ; preds = %for.inc13, %for.end
  %10 = load i32, ptr %i, align 4
  %11 = load i32, ptr %sizeB.addr, align 4
  %cmp8 = icmp slt i32 %10, %11
  br i1 %cmp8, label %for.body9, label %for.end15

for.body9:                                        ; preds = %for.cond7
  %12 = load i32, ptr %i, align 4
  %13 = load ptr, ptr %soa.addr, align 8
  %b = getelementptr inbounds %struct.StructureOne, ptr %13, i32 0, i32 1
  %14 = load i32, ptr %i, align 4
  %idxprom10 = sext i32 %14 to i64
  %arrayidx11 = getelementptr inbounds [203459 x i32], ptr %b, i64 0, i64 %idxprom10
  %15 = load i32, ptr %arrayidx11, align 4
  %call12 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %12, i32 noundef %15)
  br label %for.inc13

for.inc13:                                        ; preds = %for.body9
  %16 = load i32, ptr %i, align 4
  %inc14 = add nsw i32 %16, 1
  store i32 %inc14, ptr %i, align 4
  br label %for.cond7, !llvm.loop !10

for.end15:                                        ; preds = %for.cond7
  %call16 = call i32 (ptr, ...) @printf(ptr noundef @.str.4)
  store i32 0, ptr %i, align 4
  br label %for.cond17

for.cond17:                                       ; preds = %for.inc23, %for.end15
  %17 = load i32, ptr %i, align 4
  %18 = load i32, ptr %sizeC.addr, align 4
  %cmp18 = icmp slt i32 %17, %18
  br i1 %cmp18, label %for.body19, label %for.end25

for.body19:                                       ; preds = %for.cond17
  %19 = load i32, ptr %i, align 4
  %20 = load ptr, ptr %soa.addr, align 8
  %c = getelementptr inbounds %struct.StructureOne, ptr %20, i32 0, i32 2
  %21 = load i32, ptr %i, align 4
  %idxprom20 = sext i32 %21 to i64
  %arrayidx21 = getelementptr inbounds [315243 x i8], ptr %c, i64 0, i64 %idxprom20
  %22 = load i8, ptr %arrayidx21, align 1
  %conv = sext i8 %22 to i32
  %call22 = call i32 (ptr, ...) @printf(ptr noundef @.str.5, i32 noundef %19, i32 noundef %conv)
  br label %for.inc23

for.inc23:                                        ; preds = %for.body19
  %23 = load i32, ptr %i, align 4
  %inc24 = add nsw i32 %23, 1
  store i32 %inc24, ptr %i, align 4
  br label %for.cond17, !llvm.loop !11

for.end25:                                        ; preds = %for.cond17
  br label %for.inc26

for.inc26:                                        ; preds = %for.end25
  %24 = load i32, ptr %j, align 4
  %inc27 = add nsw i32 %24, 1
  store i32 %inc27, ptr %j, align 4
  br label %for.cond, !llvm.loop !12

for.end28:                                        ; preds = %for.cond
  ret void
}

declare dso_local i32 @printf(ptr noundef, ...) #2

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @populateAoSoA(ptr noundef %a, i32 noundef %n) #0 {
entry:
  %a.addr = alloca ptr, align 8
  %n.addr = alloca i32, align 4
  %i = alloca i32, align 4
  store ptr %a, ptr %a.addr, align 8
  store i32 %n, ptr %n.addr, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, ptr %i, align 4
  %1 = load i32, ptr %n.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load ptr, ptr %a.addr, align 8
  %3 = load i32, ptr %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds %struct.StructureOne, ptr %2, i64 %idxprom
  %a1 = getelementptr inbounds %struct.StructureOne, ptr %arrayidx, i32 0, i32 0
  %arrayidx2 = getelementptr inbounds [100345 x i32], ptr %a1, i64 0, i64 90
  store i32 100, ptr %arrayidx2, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %4 = load i32, ptr %i, align 4
  %inc = add nsw i32 %4, 1
  store i32 %inc, ptr %i, align 4
  br label %for.cond, !llvm.loop !13

for.end:                                          ; preds = %for.cond
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %n = alloca i32, align 4
  %s1 = alloca ptr, align 8
  store i32 0, ptr %retval, align 4
  store i32 1, ptr %n, align 4
  %call = call noalias ptr @malloc(i64 noundef 15304600) #6
  store ptr %call, ptr %s1, align 8
  %0 = load i32, ptr %n, align 4
  %conv = sext i32 %0 to i64
  %call1 = call noalias ptr @calloc(i64 noundef %conv, i64 noundef 1530460) #7
  store ptr %call1, ptr @gs2, align 8
  %1 = load ptr, ptr %s1, align 8
  %arrayidx = getelementptr inbounds %struct.StructureOne, ptr %1, i64 0
  %a = getelementptr inbounds %struct.StructureOne, ptr %arrayidx, i32 0, i32 0
  %arrayidx2 = getelementptr inbounds [100345 x i32], ptr %a, i64 0, i64 2
  store i32 100, ptr %arrayidx2, align 4
  %2 = load ptr, ptr %s1, align 8
  %arrayidx3 = getelementptr inbounds %struct.StructureOne, ptr %2, i64 0
  %a4 = getelementptr inbounds %struct.StructureOne, ptr %arrayidx3, i32 0, i32 0
  %arrayidx5 = getelementptr inbounds [100345 x i32], ptr %a4, i64 0, i64 2
  %3 = load i32, ptr %arrayidx5, align 4
  %call6 = call i32 (ptr, ...) @printf(ptr noundef @.str.6, i32 noundef %3)
  %4 = load ptr, ptr @gs2, align 8
  %5 = load i32, ptr %n, align 4
  call void @populateAoSoA(ptr noundef %4, i32 noundef %5)
  %6 = load ptr, ptr %s1, align 8
  call void @freeStructure(ptr noundef %6)
  %7 = load ptr, ptr @gs2, align 8
  call void @freeStructure(ptr noundef %7)
  ret i32 0
}

; Function Attrs: nounwind allocsize(0)
declare dso_local noalias ptr @malloc(i64 noundef) #3

; Function Attrs: nounwind allocsize(0,1)
declare dso_local noalias ptr @calloc(i64 noundef, i64 noundef) #4

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind allocsize(0) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind allocsize(0,1) "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }
attributes #6 = { nounwind allocsize(0) }
attributes #7 = { nounwind allocsize(0,1) }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 16.0.6 (Red Hat 16.0.6-2.module+el8.9.0+1651+e10a8f6d)"}
!4 = distinct !{!4, !5}
!5 = !{!"llvm.loop.mustprogress"}
!6 = distinct !{!6, !5}
!7 = distinct !{!7, !5}
!8 = distinct !{!8, !5}
!9 = distinct !{!9, !5}
!10 = distinct !{!10, !5}
!11 = distinct !{!11, !5}
!12 = distinct !{!12, !5}
!13 = distinct !{!13, !5}
